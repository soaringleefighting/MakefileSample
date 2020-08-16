/******************************************************************
/*    Kunpeng Technology CO. LTD
/*    2010-2020 Copyright reversed
/*    @File			:	tranpose_init_aarch64.c
/*    @Description	:	this file is initial file of tranpose_neon_aarch64.S. 
/*    @Author		:	lipeng
/*    @Modified		:	2020.8.16	Created
/********************************************************************/

void transpose_neon(unsigned char *dst, unsigned char* src, int width_lowres,
								int stride_lowres ,int lines_lowres);

void transpose_init_aarch64(unsigned char *dst, unsigned char* src, int width_lowres, int stride_lowres ,int lines_lowres)
{
	//int cpu_flags = av_get_cpu_flags();

	//if (have_neon(cpu_flags))
	{
		transpose_neon(dst, src, width_lowres, stride_lowres, lines_lowres);
	}
}