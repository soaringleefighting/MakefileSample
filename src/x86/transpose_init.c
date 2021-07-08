/******************************************************************
/*    Kunpeng Technology CO. LTD
/*    2010-2020 Copyright reversed
/*    @File			:	transpose_init.c
/*    @Description	:	this file is initial for transpose function. 
/*    @Author		:	lipeng
/*    @Modified		:	2020.8.16	Created
/********************************************************************/

#include "../../utils/cpu.h"
#include "../transpose.h"

void ff_x264_lowres_transpose_sse2(unsigned char *dst, unsigned char* src, int width,
								int stride, int lines);
void ff_x264_lowres_transpose_avx2(unsigned char *dst, unsigned char* src, int width,
								int stride, int lines);

void transpose_init_x86()
{
	int cpu_flags = av_get_cpu_flags();

	if (cpu_flags & AV_CPU_FLAG_SSE2)
	{
		printf("[AVSample_lib] info: using cpu capabilities: sse2!\n");
		transpose = ff_x264_lowres_transpose_sse2;
	}

#if ARCH_X86_64
	if (cpu_flags & AV_CPU_FLAG_AVX2)
	{
		printf("[AVSample_lib] info: using cpu capabilities: avx2!\n");
		transpose = ff_x264_lowres_transpose_avx2;
	}
#endif
}
