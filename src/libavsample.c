#include "../include/libavsample.h"
#include "transpose.h"
#include "version.h"

int libav_init()
{
	transpose_init();
	return RET_OK;
}


int libav_process(unsigned char *dst, unsigned char *src, int width_lowres,
					int stride_lowres, int lines_lowres)
{
	transpose(dst, src, width_lowres, stride_lowres, lines_lowres);
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

