/******************************************************************
    Kunpeng Technology CO. LTD
    2010-2020 Copyright reversed
    @File			:	libavsample.h
    @Description	:	this file is interface for liavsample library. 
    @Author			:	lipeng
    @Modified		:	2020.8.16	Created
********************************************************************/

#ifndef LIBAVSAMPLE_H
#define LIBAVSAMPLE_H

/* 该头文件设计目的：接口文件规范化，统一接口格式和注释格式 */

/* 返回值定义 */
#define   RET_OK    	(0)
#define   RET_FAIL		(-1)
#define   RET_MALLOC	(-2)

/* 函数接口声明 */
/******************************************************************
    Kunpeng Technology CO. LTD
    2010-2020 Copyright reversed.
    @Funtion Name		:  libav_init
    @Description		:  function pointer initial
    @Input para			:  none
	 @Output para		:  none
    @Return				:  RET_OK
    @Author				:  lipeng
    @Revison History	:
		1. Date			: 2020.8.11
		   Author		: lipeng
		   Modification	: create the function
********************************************************************/
int libav_init();

/******************************************************************
    Kunpeng Technology CO. LTD
    2010-2020 Copyright reversed.
    @Funtion Name		:  libav_process
    @Description		:  process image using transpose
    @Input para			:  dst		destination address 
						   src		source address
						   width	width of image
						   stride	stride of image
						   lines    height of image
	@Output para		:  none
    @Return				:  RET_OK
    @Author				:  lipeng
    @Revison History	:
		1. Date			: 2020.8.11
		   Author		: lipeng
		   Modification	: create the function
********************************************************************/
int libav_process(unsigned char *dst, unsigned char *src, int width,
				  int stride, int lines);

/******************************************************************
    Kunpeng Technology CO. LTD
    2010-2020 Copyright reversed.
    @Funtion Name		:  libav_uninit
    @Description		:  funtion uninit
    @Input para			:  none
	@Output para		:  none
    @Return				:  RET_OK
    @Author				:  lipeng
    @Revison History	:
		1. Date			: 2020.8.11
		   Author		: lipeng
		   Modification	: create the function
********************************************************************/
int libav_uninit();

/******************************************************************
    Kunpeng Technology CO. LTD
    2010-2020 Copyright reversed.
    @Funtion Name		:  libav_getversion
    @Description		:  get current version of library
    @Input para			:  none
	@Output para		:  none
    @Return				:  char* lib_version
    @Author				:  lipeng
    @Revison History	:
		1. Date			: 2020.8.11
		   Author		: lipeng
		   Modification	: create the function
********************************************************************/
char* libav_getversion();


#endif /*LIBAVSAMPLE_H*/
