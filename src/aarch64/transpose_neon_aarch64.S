#include "../../utils/aarch64/asm.S"
// print macro definition
.macro printf_m in1=x0, in2=x1
	st1  {\in2\().2D}, [\in1\()]
	mov  x0,  \in1
	bl     cprintf
.endm


.macro print_wx  in1=w0, in2=w1, in3=w2, in4=w3
		mov  w0, \in1
		mov  w1, \in2
		mov  w2, \in3
		mov  w3, \in4
		bl 	print
.endm

// transpose macro definition
//8x8矩阵转置
.macro transpose_8x8B  r0, r1, r2, r3, r4, r5, r6, r7, t4, t5, t6, t7
	trn1	\t4\().8B, \r0\().8B,	\r1\().8B
	trn2	\t5\().8B, \r0\().8B,	\r1\().8B
	trn1	\t6\().8B, \r2\().8B,	\r3\().8B
	trn2	\t7\().8B, \r2\().8B,	\r3\().8B

	trn1	\r0\().4H, \t4\().4H,	\t6\().4H
	trn2	\r2\().4H, \t4\().4H,	\t6\().4H
	trn1	\r1\().4H, \t5\().4H,	\t7\().4H
	trn2	\r3\().4H, \t5\().4H,	\t7\().4H
	
	trn1	\t4\().8B, \r4\().8B,	\r5\().8B
	trn2	\t5\().8B, \r4\().8B,	\r5\().8B
	trn1	\t6\().8B, \r6\().8B,	\r7\().8B
	trn2	\t7\().8B, \r6\().8B,	\r7\().8B
	
	trn1	\r4\().4H, \t4\().4H,	\t6\().4H
	trn2	\r6\().4H, \t4\().4H,	\t6\().4H
	trn1	\r5\().4H, \t5\().4H,	\t7\().4H
	trn2	\r7\().4H, \t5\().4H,	\t7\().4H
	
	trn1    \t4\().2S, \r0\().2S,   \r4\().2S
	trn2    \r4\().2S, \r0\().2S,   \r4\().2S
	trn1    \t5\().2S, \r1\().2S,   \r5\().2S
	trn2    \r5\().2S, \r1\().2S,   \r5\().2S
	
	trn1    \t6\().2S, \r2\().2S,   \r6\().2S
	trn2    \r6\().2S, \r2\().2S,   \r6\().2S
	trn1    \t7\().2S, \r3\().2S,   \r7\().2S
	trn2    \r7\().2S, \r3\().2S,   \r7\().2S
	
	mov		\r0\().8B, \t4\().8B
	mov		\r1\().8B, \t5\().8B
	mov		\r2\().8B, \t6\().8B
	mov		\r3\().8B, \t7\().8B
	//printf_m	x0, \t5
.endm



/*
//for (int y = 0; y < frame->i_width_lowres; y++, dst += frame->i_stride_lowres) // width
//    for (int x = 0; x < frame->i_lines_lowres; x++) // height
//        dst[x] = frame->lowres[i][(frame->i_lines_lowres - x -1) * frame->i_stride_lowres + y];
*/
// dst: 			x0
// src: 			x1	
// width_lowres: 	x2
// stride_lowres: 	x3
// lines_lowres:    x4
// C代码转置的思想：目标矩阵的第一行是原始矩阵的第一列的逆向。
// example:
// src：   1, 2, 3, 4,  	// dst: 13, 9, 	5, 1
//         5, 6, 7, 8,              14, 10, 6, 2
//         9, 10,11,12,				15, 11, 7, 3
//		   13,14,15,16		        16, 12, 8, 4
// 优化思路：将图像分割成一系列8x8小矩阵，对每个小矩阵分别进行转置;原始图像是连续存储的(先列后行)，
// 转置后的目标图像坐标需要计算得到。
// TODO:考虑不是8的倍数情况下，边角的处理方式。
function x264_lowres_transpose_neon, export=1
		mov			x5,  x1			// 暂存r1 src
		mov			x6,  x0			// 暂存r0 dst
		mov			x7,  x4			// 暂存r4 lines
		//print_wx  	w2, w3, w4, w4
		
main_loop_row_64:		
		// dst位置计算
		add         x0,   x0,   x7
		
main_loop_64:
		// 主处理
		// 1.取出8x8块原始像素
		ld1		{v7.8B}, [x1], x3
		ld1		{v6.8B}, [x1], x3
		ld1		{v5.8B}, [x1], x3
		//printf_m	x0, v2
		ld1		{v4.8B}, [x1], x3
		ld1		{v3.8B}, [x1], x3
		ld1		{v2.8B}, [x1], x3
		ld1		{v1.8B}, [x1], x3
		ld1		{v0.8B}, [x1], x3
		
		// 2.8x8转置 
		transpose_8x8B v0, v1, v2, v3, v4, v5, v6, v7, v16, V17, v18, v19
		//printf_m	  x0, v0
		
		//2.1向量元素翻转
		/*
		rev64	v0.8B, v0.8B
		rev64	v1.8B, v1.8B
		rev64	v2.8B, v2.8B
		rev64	v3.8B, v3.8B
		rev64	v4.8B, v4.8B
		rev64	v5.8B, v5.8B
		rev64	v6.8B, v6.8B		
		rev64	v7.8B, v7.8B
		*/
		
		// 3.存储到对应位置
		// 3.1.dst位置计算
		sub			x0,  x0,  #8
		// 3.2.目标像素存储 特别注意dst的跨度是lines_lowres
		st1			{v0.8B}, [x0], x7
		st1			{v1.8B}, [x0], x7
		st1			{v2.8B}, [x0], x7
		st1			{v3.8B}, [x0], x7
		st1			{v4.8B}, [x0], x7
		st1			{v5.8B}, [x0], x7
		st1			{v6.8B}, [x0], x7
		st1			{v7.8B}, [x0], x7
		sub			x0, x0, x7, lsl  #3
		
		
		// 4.循环处理
		subs		x4, x4, #8
		b.gt		main_loop_64
		
		mov     	x4,  x7		//恢复r4
		// 4.1目标地址重新计算
		mov			x0, x6
		add         x0, x0, x7, lsl #3
		mov			x6, x0
		// 4.2原始地址重新计算
		mov			x1, x5
		add			x1, x1, #8
		mov			x5, x1
		
		subs		x2, x2, #8
		b.gt		main_loop_row_64
		
		ret
endfunc