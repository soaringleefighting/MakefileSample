/******************************************************************
/*    Kunpeng Technology CO. LTD
/*    2010-2020 Copyright reversed
/*    @File			:	tranpose_init_arm.c
/*    @Description	:	this file is initial file of tranpose_neon_arm.S. 
/*    @Author		:	lipeng
/*    @Modified		:	2020.8.16	Created
/********************************************************************/

//#include "../../utils/arm/cpu.h"
#include "../transpose.h"

void x264_lowres_transpose_neon(unsigned char *dst, unsigned char* src, int width_lowres, int stride_lowres ,int lines_lowres);


void x264_lowres_transpose_init_arm(unsigned char *dst, unsigned char* src, int width_lowres, int stride_lowres ,int lines_lowres)
{
	//int cpu_flags = av_get_cpu_flags();

	//if (have_neon(cpu_flags))
	{
		x264_lowres_transpose_neon(dst, src, width_lowres, stride_lowres, lines_lowres);
	}
}
