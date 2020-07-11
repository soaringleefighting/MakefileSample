#ifndef LIBAVSAMPLE_H
#define LIBAVSAMPLE_H

// 该头文件设计目的：接口文件规范化，统一接口格式和注释格式

// 返回值定义
#define   RET_OK    	(1)
#define   RET_FAIL		(-1)
#define   RET_MALLOC	(-2)

// 函数接口声明
//***********************************************
// Funtion: libav_init
// Parameter:
//
// Usage:
// Modified: lipeng created at July 11 2020
//************************************************

void libav_init();

//***********************************************
// Funtion: libav_process
// Parameter:
//
// Usage:
// Modified: lipeng created at July 11 2020
//************************************************
void libav_process(unsigned char *dst, unsigned char *src, int width_lowres,
					int stride_lowres, int lines_lowres);

//***********************************************
// Funtion: libav_uninit
// Parameter:
//
// Usage:
// Modified: lipeng created at July 11 2020

//************************************************
void libav_uninit();

#endif /*LIBAVSAMPLE_H*/
