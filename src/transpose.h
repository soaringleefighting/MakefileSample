#ifndef X264_LOWRES_TRANSPOSE_H
#define X264_LOWRES_TRANSPOSE_H

#include <stdio.h>

// 函数指针的声明
void (*transpose)(unsigned char *dst, unsigned char *src, int width_lowres, 
					int stride_lowres, int lines_lowres);

void transpose_init(unsigned char *dst, unsigned char *src, int width_lowres, 
					int stride_lowres, int lines_lowres);


void libavsample_transpose_c(unsigned char *dst, unsigned char *src, int width_lowres,
					int stride_lowres, int lines_lowres);

// aarch64 neon初始化
void transpose_init_aarch64(unsigned char *dst, unsigned char *src, int width_lowres,
					int stride_lowres, int lines_lowres);

// arm neon初始化
void transpose_init_arm(unsigned char *dst, unsigned char *src, int width_lowres, 
					int stride_lowres, int lines_lowres);

// x86 simd初始化
void transpose_init_x86(unsigned char *dst, unsigned char *src, int width_lowres,
					int stride_lowres, int lines_lowres);
#endif /*X264_LOWRES_TRANSPOSE_H*/
