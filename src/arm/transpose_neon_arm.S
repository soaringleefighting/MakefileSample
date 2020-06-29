#include "../../utils/arm/asm.S"
// print macro definition
.macro print_m in1=r0, in2=d0
       push {r0-r3, lr}
       vst1.u64       {\in2\()}, [\in1\()]
       mov     r0, \in1
       bl cprintf
       pop {r0-r3, pc}
.endm

.macro print_rx in1=r0, in2=r1, in3=r2, in4=r3 
		push {r0-r3, lr}
		mov	  r0, \in1
		mov	  r1, \in2
		mov	  r2, \in3
		mov	  r3, \in4
		bl print
		pop  {r0-r3, pc}
.endm

// transpose macro definition
.macro transpose_4x4 rq0, rq1, r0, r1, r2, r3
        vtrn.16        \rq0, \rq1
        vtrn.8         \r0,  \r1
        vtrn.8         \r2,  \r3
.endm

.macro transpose8_4x4 r0, r1, r2, r3
        vtrn.8         \r0,  \r1
        vtrn.8         \r2,  \r3
        vtrn.16        \r0,  \r2
        vtrn.16        \r1,  \r3
.endm

.macro transpose_8x8 r0, r1, r2, r3, r4, r5, r6, r7
        transpose8_4x4  \r0, \r1, \r2, \r3
        transpose8_4x4  \r4, \r5, \r6, \r7
		vtrn.32		    \r0,  \r4
		vtrn.32		    \r1,  \r5
		vtrn.32		    \r2,  \r6
		vtrn.32		    \r3,  \r7
.endm


/*
//for (int y = 0; y < frame->i_width_lowres; y++, dst += frame->i_stride_lowres) // width
//    for (int x = 0; x < frame->i_lines_lowres; x++) // height
//        dst[x] = frame->lowres[i][(frame->i_lines_lowres - x -1) * frame->i_stride_lowres + y];
*/
// dst: 			r0
// src: 			r1	
// width_lowres: 	r2
// stride_lowres: 	r3
// lines_lowres:    r4
// C代码转置的思想：目标矩阵的第一行是原始矩阵的第一列的逆向。
// example:
// src：   1, 2, 3, 4,  	// dst: 13, 9, 	5, 1
//         5, 6, 7, 8,              14, 10, 6, 2
//         9, 10,11,12,				15, 11, 7, 3
//		   13,14,15,16		        16, 12, 8, 4
// 优化思路：将图像分割成一系列8x8小矩阵，对每个小矩阵分别进行转置;转置后图像是连续存储的(先行后列)，
// 原始图像坐标需要计算得到。
// TODO:考虑不是8的倍数情况下，边角的处理方式。
function x264_lowres_transpose_neon, export=1
		push		{r4-r7}
		ldr			r4,	[sp, #16]	// lines_lowres 高度
		mov			r5,  r1			// 暂存r1 src
		mov			r6,  r0			// 暂存r0 dst
		mov			r7,  r4			// 暂存r4 lines
		//print_rx  	r2, r3, r4
		
main_loop_row:
		// dst位置计算
		add         r0,   r0,   r7
		
main_loop:
		// 主处理
		// 1.取出8x8块原始像素
		vld1.8		{d7}, [r1], r3
		vld1.8		{d6}, [r1], r3
		vld1.8		{d5}, [r1], r3
		//print_m		r0, d3
		vld1.8		{d4}, [r1], r3
		vld1.8		{d3}, [r1], r3
		vld1.8		{d2}, [r1], r3
		vld1.8		{d1}, [r1], r3
		vld1.8		{d0}, [r1], r3
		
		// 2.8x8转置
		transpose_8x8 d0, d1, d2, d3, d4, d5, d6, d7
		//print_m		r0, d7
		
		//2.1翻转
		/*
		vrev64.8	d0, d0
		vrev64.8	d1, d1
		vrev64.8	d2, d2
		vrev64.8	d3, d3
		vrev64.8	d4, d4
		vrev64.8	d5, d5
		vrev64.8	d6, d6		
		vrev64.8	d7, d7
		*/
		
		// 3.存储到对应位置
		// 3.1.dst位置计算
		sub			r0,  r0,  #8
		// 3.2.目标像素存储 特别注意dst的跨度是lines_lowres
		vst1.8		{d0}, [r0], r7
		vst1.8		{d1}, [r0], r7
		vst1.8		{d2}, [r0], r7
		vst1.8		{d3}, [r0], r7
		vst1.8		{d4}, [r0], r7
		vst1.8		{d5}, [r0], r7
		vst1.8		{d6}, [r0], r7
		vst1.8		{d7}, [r0], r7
		sub			r0, r0, r7, lsl  #3
		
		// 4.循环处理
		subs		r4, r4, #8
		bgt			main_loop
		
		mov     	r4,  r7		//恢复r4
		// 4.1目标地址重新计算
		mov			r0, r6
		add         r0, r0, r7, lsl #3
		mov			r6, r0
		// 4.2原始地址重新计算
		mov			r1, r5
		add			r1, r1, #8
		mov			r5, r1
		
		subs		r2, r2, #8
		bgt			main_loop_row
		
		pop			{r4-r7}
		bx			lr
endfunc