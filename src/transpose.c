/******************************************************************
/*    Kunpeng Technology CO. LTD
/*    2010-2020 Copyright reversed
/*    @File			:	transpose.c
/*    @Description	:	this file used to tranpose image. 
/*    @Author		:	lipeng
/*    @Modified		:	2020.8.16	Created
/********************************************************************/

#include "transpose.h"

int libavsample_transpose_c(unsigned char *dst, unsigned char *src, int width,
							int stride, int lines)
{
	int y = 0, x = 0;

	/* input parameter check */
	if (NULL == dst || NULL == src || width > INT_MAX ||
		stride > INT_MAX || lines > INT_MAX)
	{
		return -1;
	}

	for (y = 0; y < width; y++, dst += lines) // width
	{
           for (x = 0; x < lines; x++) // height
		   {
               dst[x] = src[(lines-x-1)*stride + y];
		   }
	}

	return 0;
}

/**
 * Set transpose function pointers.
 */
void transpose_init()
{
	transpose = libavsample_transpose_c;

	// 支持根据不同平台设置汇编函数指针
#if ARCH_AARCH64
		transpose_init_aarch64();
#endif // End of #if ARCH_AARCH64

#if ARCH_ARM
		transpose_init_arm();
#endif // End of #if ARCH_ARM

#if ARCH_X86
		transpose_init_x86();
#endif // End of #if ARCH_X86

#if ARCH_MIPS
	{
		//TODO
	}
#endif // End of #if ARCH_MIPS
}
