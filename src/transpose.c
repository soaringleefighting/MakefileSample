#include "transpose.h"

void libavsample_transpose_c(unsigned char *dst, unsigned char *src, int width_lowres, int stride_lowres, int lines_lowres)
{
	int y, x;
	for (y = 0; y < width_lowres; y++, dst += lines_lowres) // width 
           for (x = 0; x < lines_lowres; x++) // height
               dst[x] = src[(lines_lowres-x-1)*stride_lowres + y];
}

/**
 * Set transpose function pointers.
 */
void transpose_init()
{
	transpose = libavsample_transpose_c;

	// TODO: 支持根据不同平台设置汇编函数指针
// 	if (ARCH_AARCH64)
// 	{
// 		transpose_init_aarch64(dst, src, width_lowres, stride_lowres, lines_lowres);
// 	}
// 
// 	if (ARCH_ARM)
// 	{
// 		transpose_init_arm(dst, src, width_lowres, stride_lowres, lines_lowres);
// 	}
// 
// 	if (ARCH_X86)
// 	{
// 		transpose_init_x86(dst, src, width_lowres, stride_lowres, lines_lowres);
// 	}
// 
// 	if (ARCH_MIPS)
// 	{
// 		//TODO
// 	}
}
