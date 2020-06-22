%include "../../utils/x86/x86util.asm"

section .text

;----------------------------------------------------------------------------
;void ff_x264_lowres_transpose_sse2(unsigned char *dst,
; unsigned char* src, int width_lowres, int stride_lowres ,int lines_lowres);
;----------------------------------------------------------------------------
; dst : 		 r0
; src : 		 r1
; width_lowres:  r2
; stride_lowres: r3
; lines_lowres:  r4
; 优化思路：将图像分割成一系列8x8小矩阵，对每个小矩阵分别进行转置;转置后图像是连续存储的(先行后列)，
; 原始图像坐标需要计算得到。
; Modified: lipeng created 2020.6.11
;==============================================================================
INIT_XMM sse2
cglobal x264_lowres_transpose,5, 6, 8, 0x10, dst, src, width, stride, lines, linesb
		movzx	 linesq, linesw		; 高位置零
		mov	 linesbq, linesq	
		mov  r8m, srcq

.loop_row
		add	 dstq, linesq		; dst位置计算
.main_loop:
		; 主处理函数
		;1.取出8x8块原始像素值
		movq m7,  [srcq]
		add srcq, strideq
		movq m6,  [srcq]
		add srcq, strideq
		movq m5,  [srcq]
		add srcq, strideq
		movq m4,  [srcq]
		add srcq, strideq
		movq m3,  [srcq]
		add srcq, strideq
		movq m2,  [srcq]
		add srcq, strideq
		movq m1,  [srcq]
		add srcq, strideq
		movq m0,  [srcq]
		add srcq, strideq
		
		; 2.8x8转置
		TRANSPOSE_8X8B 0, 1, 2, 3, 4, 5, 6, 7
			
		; 3.存储到对应位置
		; 3.1 dst位置计算
		sub  dstq, 8
		; 3.2.目标像素存储 特别注意dst的跨度是lines_lowres
		movq	[dstq], 	m0
		add		dstq,  linesbq
		movq    [dstq], 	m1
		add		dstq,  linesbq
		movq    [dstq],  	m2
		add		dstq,  linesbq
		movq    [dstq],  	m3
		add		dstq,  linesbq
		movq    [dstq], 	m4
		add		dstq,  linesbq
		movq    [dstq],  	m5
		add		dstq,  linesbq
		movq    [dstq], 	m6
		add		dstq,  linesbq
		movq    [dstq], 	m7
		;恢复到刚开始一行
		sub	    dstq, linesbq
		sub	    dstq, linesbq
		sub	    dstq, linesbq
		sub	    dstq, linesbq
		sub	    dstq, linesbq
		sub	    dstq, linesbq
		sub	    dstq, linesbq

		;4. 循环处理
		sub     linesd, 8
		jg		.main_loop
		; 处理列
		mov     linesq,  linesbq  ; 恢复linesd
		
		; 4.1目标地址重新计算
		lea   dstq, [dstq+linesq*8]
		 
		; 4.2原始地址重新计算
		;恢复src
		mov     srcq, r8m
		add     srcq, 8
		mov		r8m, srcq
		
		sub  	widthq, 8
		jg	 .loop_row
RET

