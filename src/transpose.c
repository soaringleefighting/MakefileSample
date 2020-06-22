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
void transpose_init(unsigned char *dst, unsigned char *src, int width_lowres, int stride_lowres, int lines_lowres)
{
	transpose = libavsample_transpose_c;
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
