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
; 要求：图像宽和高都是8的倍数
; Modified: lipeng created 2020.6.11
;==============================================================================
INIT_XMM sse2
cglobal x264_lowres_transpose,5,13,8, dst, src, width, stride, lines, stride3,stride5,stride7, lines3, lines5, lines7, linesb, srcb
		movzx	 linesq, linesw		; 高位置零
		mov	 linesbq, linesq
		mov  srcbq, srcq
		lea  stride3q, [strideq*3]
		lea  stride5q, [strideq*5]
		lea  stride7q, [stride5q+strideq*2]
		
		lea  lines5d, [linesd*5]
		lea  lines7d, [lines5d+linesd*2]	
.loop_row
		add	 dstq, linesq		; dst位置计算
		lea  lines3d, [linesd*3]
.main_loop:
		; 主处理函数
		;1.取出8x8块原始像素值
		movq m7,  [srcq]
		movq m6,  [srcq+strideq]
		movq m5,  [srcq+strideq*2]
		movq m4,  [srcq+stride3q]
		movq m3,  [srcq+strideq*4]
		movq m2,  [srcq+stride5q]
		movq m1,  [srcq+stride3q*2]
		movq m0,  [srcq+stride7q]
		lea	srcq, [srcq+strideq*8]
		
		; 2.8x8转置
		TRANSPOSE_8X8B 0, 1, 2, 3, 4, 5, 6, 7
			
		; 3.存储到对应位置
		; 3.1 dst位置计算
		sub  dstq, 8
		; 3.2.目标像素存储 特别注意dst的跨度是lines_lowres
		movq	[dstq], 			m0
		movq    [dstq+linesbq], 	m1
		movq    [dstq+linesbq*2],  	m2
		movq    [dstq+lines3q],  	m3
		movq    [dstq+linesbq*4], 	m4
		movq    [dstq+lines5q],  	m5
		movq    [dstq+lines3q*2], 	m6
		movq    [dstq+lines7q], 	m7
		
		;4. 循环处理
		sub     linesd, 8
		jg		.main_loop
		; 处理列
		mov     linesq,  linesbq  ; 恢复linesd
		
		; 4.1目标地址重新计算
		lea  	lines3d, [linesd*8]
		add		dstq, lines3q
		; 4.2原始地址重新计算
		;恢复src
		mov     srcq, srcbq
		add     srcq, 8
		mov		srcbq, srcq
		
		sub  	widthq, 8
		jg	 .loop_row
RET


;----------------------------------------------------------------------------
;void ff_x264_lowres_transpose_avx2(unsigned char *dst,
; unsigned char* src, int width_lowres, int stride_lowres ,int lines_lowres);
;----------------------------------------------------------------------------
; dst : 		 r0
; src : 		 r1
; width_lowres:  r2
; stride_lowres: r3
; lines_lowres:  r4
; 优化思路：将图像分割成一系列16x16小矩阵，对每个小矩阵分别进行转置;转置后图像是连续存储的(先行后列)，
; 原始图像坐标需要计算得到。
; 要求：处理的图像宽和高都是16的倍数；非16倍数的图像边界处理在调用函数中进行。
; Modified: lipeng created 2020.6.16
;==============================================================================
INIT_YMM avx2
cglobal x264_lowres_transpose,5,14,16, 0x20, dst, src, width, stride, lines, stride3,stride5, stride7, lines3, lines5, lines7, linesb, srcb, dstbc
		movzx	 linesq, linesw		; 高位置零
		mov	 linesbq, linesq		; 保存lines
		mov  srcbq, srcq			; 保存src
		mov	 dstbcq, dstq			; 保存dst 	
		lea  stride3q, [strideq*3]
		lea  stride5q, [strideq*5]
		lea  stride7q, [stride5q+strideq*2]
		lea  lines5d, [linesd*5]
		lea  lines7d, [lines5d+linesd*2]	
.loop_row_avx2
		add	 dstq, linesbq		; dst位置计算
		lea  lines3q, [linesbq*3]
.main_loop_avx2:
		; 主处理函数
		;1.取出16x16块原始像素值
		movdqu m7,  [srcq]
		movdqu m6,  [srcq+strideq]
		movdqu m5,  [srcq+strideq*2]
		movdqu m4,  [srcq+stride3q]
		movdqu m3,  [srcq+strideq*4]
		movdqu m2,  [srcq+stride5q]
		movdqu m1,  [srcq+stride3q*2]
		movdqu m0,  [srcq+stride7q]
		lea  srcq, [srcq+strideq*8]
		
		; 2.16x16转置
		;TRANSPOSE_16X16B 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18
		SBUTTERFLY bw, 0,  1,  9
		SBUTTERFLY bw, 2,  3,  9
		SBUTTERFLY bw, 4,  5,  9
		SBUTTERFLY bw, 6,  7,  9		
	
		SBUTTERFLY wd, 0,  2,  9
		SBUTTERFLY wd, 1,  3,  9		
		SBUTTERFLY wd, 4,  6,  9
		SBUTTERFLY wd, 5,  7,  9
		
		SBUTTERFLY dq, 0, 4,  9
		SBUTTERFLY dq, 2, 6,  9
		SBUTTERFLY dq, 1, 5,  9
		SBUTTERFLY dq, 3, 7,  9
		
		movdqu m15,   [srcq]
		movdqu m14,   [srcq+strideq]
		movdqu m13,  [srcq+strideq*2]
		movdqu m12,  [srcq+stride3q]
		movdqu m11,  [srcq+strideq*4]
		movdqu m10,  [srcq+stride5q]
		movdqu m9,  [srcq+stride3q*2]
		movdqu m8,  [srcq+stride7q]
		lea  srcq, [srcq+strideq*8]
		;lea	srcq, 	[srcq+stride3q*2]
		;add srcq, strideq
		;movdqu m8,  [srcq]
		;add srcq, strideq

		
		movdqu		[rsp + 16] , m0	; 保存m0

		SBUTTERFLY bw, 8,  9,   0
		SBUTTERFLY bw, 10, 11,  0
		SBUTTERFLY bw, 12, 13,  0
		SBUTTERFLY bw, 14, 15,  0		
	
		SBUTTERFLY wd, 8,  10,  0
		SBUTTERFLY wd, 9,  11,  0		
		SBUTTERFLY wd, 12, 14,  0
		SBUTTERFLY wd, 13, 15,  0
		
		SBUTTERFLY dq, 8, 12,  0
		SBUTTERFLY dq, 10,14,  0
		SBUTTERFLY dq, 9, 13,  0
		SBUTTERFLY dq, 11,15,  0
		
		movdqu		m0, [rsp + 16]    ; 恢复m0

		movdqu		[rsp + 16], m1	; 保存m1
		
		SBUTTERFLY qdq, 8, 0,  	1
		SBUTTERFLY qdq, 12, 4,  1
		SBUTTERFLY qdq, 10, 2,  1
		SBUTTERFLY qdq, 14, 6,  1		
		
		movdqu		m1, [rsp + 16]	; 恢复m1
		
		; 3.存储到对应位置
		; 3.1 dst位置计算
		sub  dstq, 16
		
		; 先存储部分数据
		; 3.2.目标像素存储 特别注意dst的跨度是lines_lowres
		movdqu	[dstq], 			xmm8
		movdqu  [dstq+linesbq], 	xmm0
		movdqu  [dstq+linesbq*2],  	xmm12
		movdqu  [dstq+lines3q],  	xmm4
		movdqu  [dstq+linesbq*4], 	xmm10
		movdqu  [dstq+lines5q],  	xmm2
		movdqu  [dstq+lines3q*2], 	xmm14
		movdqu  [dstq+lines7q], 	xmm6	
		
		SBUTTERFLY qdq, 9, 1,   0
		SBUTTERFLY qdq, 13, 5,  0
		SBUTTERFLY qdq, 11, 3,  0
		SBUTTERFLY qdq, 15, 7,  0

		lea		dstq, [dstq+linesbq*8]
		movdqu	[dstq], 			xmm9
		movdqu  [dstq+linesbq], 	xmm1
		movdqu  [dstq+linesbq*2],  	xmm13
		movdqu  [dstq+lines3q],  	xmm5
		movdqu  [dstq+linesbq*4], 	xmm11
		movdqu  [dstq+lines5q],  	xmm3
		movdqu  [dstq+lines3q*2], 	xmm15
		movdqu  [dstq+lines7q], 	xmm7
		sub		dstq, lines7q
		sub		dstq, linesbq

		;4. 循环处理
		sub     linesq, 16
		cmp		linesq, 16
		jge		.main_loop_avx2
		; 处理列
		mov     linesq,  linesbq  ; 恢复linesd
		
		; 4.1目标地址重新计算
		lea  	lines3q, [linesq*8]
		mov		dstq, dstbcq
		add		dstq, lines3q
		add		dstq, lines3q
		mov		dstbcq, dstq
		; 4.2原始地址重新计算
		;恢复src
		mov     srcq, srcbq
		add     srcq, 16
		mov		srcbq, srcq
		
		sub  	widthq, 16
		cmp     widthq, 16
		jge	 .loop_row_avx2
RET