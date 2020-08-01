#ifndef LIBAVSAMPLE_H
#define LIBAVSAMPLE_H

// 该头文件设计目的：接口文件规范化，统一接口格式和注释格式

// 返回值定义
#define   RET_OK    	(0)
#define   RET_FAIL		(-1)
#define   RET_MALLOC	(-2)

// 函数接口声明
//***********************************************
// Funtion: libav_init
// Parameter: none
// Usage: function pointer initial
// Modified: lipeng created at July 11 2020
//************************************************

int libav_init();

//***********************************************
// Funtion: libav_process
// Parameter: 
//			dst		destination address
//			src		source address
//			width	width of image
//			stride	stride of image
//			lines   height of image
// Usage: process image using transpose
// Modified: lipeng created at July 11 2020
//************************************************
int libav_process(unsigned char *dst, unsigned char *src, int width_lowres,
					int stride_lowres, int lines_lowres);

//***********************************************
// Funtion: libav_uninit
// Parameter: none
// Usage: funtion uninit
// Modified: lipeng created at July 11 2020
//************************************************
int libav_uninit();


//***********************************************
// Funtion: libav_getversion
// Parameter: none
// Usage: get current version of library
// Modified: lipeng created at August 1 2020
//************************************************
char* libav_getversion();


#endif /*LIBAVSAMPLE_H*/
