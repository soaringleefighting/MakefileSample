/******************************************************************
    Kunpeng Technology CO. LTD
    2010-2020 Copyright reversed
    @File			:	tranpose_init_arm.c
    @Description	:	this file is initial file of tranpose_neon_arm.S. 
    @Author			:	lipeng
    @Modified		:	2020.8.16	Created
********************************************************************/

#include "../../utils/cpu.h"
#include "../transpose.h"

int x264_lowres_transpose_neon(unsigned char *dst, unsigned char* src, int width_lowres, int stride_lowres ,int lines_lowres);


void transpose_init_arm()
{
	int cpu_flags = av_get_cpu_flags();

	if (cpu_flags & AV_CPU_FLAG_NEON)
	{
		printf("[AVSample_lib] info: using cpu capabilities: neon!\n");
		transpose = x264_lowres_transpose_neon;
	}
}
