/******************************************************************
/*    Kunpeng Technology CO. LTD
/*    2010-2020 Copyright reversed
/*    @File			:	libavsample.c
/*    @Description	:   this file is interface for transpose function. 
/*    @Author		:	lipeng
/*    @Modified		:	2020.8.16	Created
/********************************************************************/

#include "../include/libavsample.h"
#include "transpose.h"
#include "version.h"

int libav_init()
{
	transpose_init();
	return RET_OK;
}


int libav_process(unsigned char *dst, unsigned char *src, int width,
					int stride, int lines)
{
	transpose(dst, src, width, stride, lines);
	return RET_OK;
}

int  libav_uninit()
{
	return RET_OK;
}

char lib_version[255]; //定义成全局变量，用于返回数组地址//TODO:考虑这种方法的优劣?
char* libav_getversion()
{
	sprintf(lib_version, "date: %s, time: %s, version: %s.%s.%s",
		__DATE__,
		__TIME__,
		MAJOR_VERSION,
		MINOR_VERSION,
		SG_VERSION);
	return lib_version;
}

