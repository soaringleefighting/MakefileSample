/******************************************************************
/*    Kunpeng Technology CO. LTD
/*    2010-2020 Copyright reversed
/*    @File			:	transpose_init.c
/*    @Description	:	this file is initial for transpose function. 
/*    @Author		:	lipeng
/*    @Modified		:	2020.8.16	Created
/********************************************************************/

//#include "../../utils/x86/cpu.h"
#include "../transpose.h"

void ff_x264_lowres_transpose_sse2(unsigned char *dst, unsigned char* src, int width,
								int stride, int lines);
void ff_x264_lowres_transpose_avx2(unsigned char *dst, unsigned char* src, int width,
								int stride, int lines);

void transpose_init_x86(unsigned char *dst, unsigned char* src, int width,	 
						int stride ,int lines)
{
	//int cpu_flags = av_get_cpu_flags();

	//if (EXTERNAL_SSE2(cpu_flags))
	{

		//printf("cpu feature sse2!!!\n");
		ff_x264_lowres_transpose_sse2(dst, src, width, stride, lines);
	}

#if ARCH_X86_64
	//if (EXTERNAL_AVX2(cpu_flags))
	{
		//printf("cpu feature avx2!!!\n");
		ff_x264_lowres_transpose_avx2(dst, src, width, stride, lines);
	}
#endif
}
