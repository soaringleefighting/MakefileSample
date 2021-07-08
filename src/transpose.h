/******************************************************************
    Kunpeng Technology CO. LTD
    2010-2020 Copyright reversed
    @File			:	tranpose.h
    @Description	:	this file is header file of tranpose.c. 
    @Author			:	lipeng
    @Modified		:	2020.8.16	Created
********************************************************************/

#ifndef X264_LOWRES_TRANSPOSE_H
#define X264_LOWRES_TRANSPOSE_H

#include <stdio.h>
#include <string.h>
#include <limits.h>

/* 函数指针的声明 */
/******************************************************************
    Kunpeng Technology CO. LTD
    2010-2020 Copyright reversed.
    @Funtion Name		:  transpose
    @Description		:  declartion of funtion pointer
    @Input para			:  src		source address
						   width	width of image
						   stride	stride of image
						   lines    height of image
	@Output para		:  dst		destination address 
    @Return				:  none
    @Author				:  lipeng
    @Revison History	:
		1. Date			: 2020.8.11
		   Author		: lipeng
		   Modification	: create the function
********************************************************************/
int (*transpose)(unsigned char *dst, unsigned char *src, int width, 
				  int stride, int lines);

/******************************************************************
    Kunpeng Technology CO. LTD
    2010-2020 Copyright reversed.
    @Funtion Name		:  transpose_init
    @Description		:  initial of funtion pointer
    @Input para			:  none
	@Output para		:  none
    @Return				:  none
    @Author				:  lipeng
    @Revison History	:
		1. Date			: 2020.8.11
		   Author		: lipeng
		   Modification	: create the function
********************************************************************/
void transpose_init();

/******************************************************************
    Kunpeng Technology CO. LTD
    2010-2020 Copyright reversed.
    @Funtion Name		:  libavsample_transpose_c
    @Description		:  C implementation of transpose funtion
    @Input para			:  src		source address
						   width	width of image
						   stride	stride of image
						   lines    height of image
	@Output para		:  dst		destination address 
    @Return				:  0: Success   -1: Failed
    @Author				:  lipeng
    @Revison History	:
		1. Date			: 2020.8.11
		   Author		: lipeng
		   Modification	: create the function
********************************************************************/
int libavsample_transpose_c(unsigned char *dst, unsigned char *src, int width,
							 int stride, int lines);

// aarch64 neon初始化
/******************************************************************
    Kunpeng Technology CO. LTD
    2010-2020 Copyright reversed.
    @Funtion Name		:  transpose_init_aarch64
    @Description		:  aarch64 neon optimization of transpose funtion
    @Input para			:  src		source address
						   width	width of image
						   stride	stride of image
						   lines    height of image
	@Output para		:  dst		destination address 
	@Output para		:  none
    @Return				:  none
    @Author				:  lipeng
    @Revison History	:
		1. Date			: 2020.8.11
		   Author		: lipeng
		   Modification	: create the function
********************************************************************/
void transpose_init_aarch64();

// arm neon初始化
/******************************************************************
    Kunpeng Technology CO. LTD
    2010-2020 Copyright reversed.
    @Funtion Name		:  transpose_init_arm
    @Description		:  arm neon optimization of transpose funtion
    @Input para			:  src		source address
						   width	width of image
						   stride	stride of image
						   lines    height of image
	@Output para		:  dst		destination address 
	@Output para		:  none
    @Return				:  none
    @Author				:  lipeng
    @Revison History	:
		1. Date			: 2020.8.11
		   Author		: lipeng
		   Modification	: create the function
********************************************************************/
void transpose_init_arm();

// x86 simd初始化
/******************************************************************
    Kunpeng Technology CO. LTD
    2010-2020 Copyright reversed.
    @Funtion Name		:  transpose_init_arm
    @Description		:  X86 SIMD optimization of transpose funtion
    @Input para			:  src		source address
						   width	width of image
						   stride	stride of image
						   lines    height of image
	@Output para		:  dst		destination address 
	@Output para		:  none
    @Return				:  none
    @Author				:  lipeng
    @Revison History	:
		1. Date			: 2020.8.11
		   Author		: lipeng
		   Modification	: create the function
********************************************************************/
void transpose_init_x86();

#endif /*X264_LOWRES_TRANSPOSE_H*/
