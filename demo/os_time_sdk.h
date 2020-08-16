/******************************************************************
/*    Kunpeng Technology CO. LTD
/*    2010-2020 Copyright reversed
/*    @File			:	os_time_sdk.c
/*    @Description	:	this file is interface for time sdk. 
/*    @Author		:	lipeng
/*    @Modified		:	2020.8.16	Created
/********************************************************************/

#ifndef _OS_TIME_SDK_H_
#define _OS_TIME_SDK_H_

#ifdef __cplusplus
extern "C"{
#endif

#if defined(WIN32) || defined(UNDER_CE) || defined(WIN64)
#include <windows.h>
#include <io.h>
typedef struct _os_timer
{
   LARGE_INTEGER lpFrequency;
   LARGE_INTEGER tStart;
   LARGE_INTEGER tEnd;
}os_timer;

#elif defined(__GNUC__)
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
typedef struct _os_timer
{
  struct timeval tStart;
  struct timeval tEnd;
}os_timer;
#endif

/******************************************************************
/*    Kunpeng Technology CO. LTD
/*    2010-2020 Copyright reversed.
/*    @Funtion Name		:  os_sdk_inittimer
/*    @Description		:  function initial
/*    @Input para		:  os_timer *pTimer
/*	  @Output para		:  none
/*    @Return			:  0
/*    @Author			:  lipeng
/*    @Revison History	:
/*		1. Date			: 2020.8.11
/*		   Author		: lipeng
/*		   Modification	: create the function
/********************************************************************/
int os_sdk_inittimer(os_timer *pTimer);

/******************************************************************
/*    Kunpeng Technology CO. LTD
/*    2010-2020 Copyright reversed.
/*    @Funtion Name		:  os_sdk_starttimer
/*    @Description		:  start timer
/*    @Input para		:  os_timer *pTimer
/*	  @Output para		:  none
/*    @Return			:  0
/*    @Author			:  lipeng
/*    @Revison History	:
/*		1. Date			: 2020.8.11
/*		   Author		: lipeng
/*		   Modification	: create the function
/********************************************************************/
int os_sdk_starttimer(os_timer *pTimer);

/******************************************************************
/*    Kunpeng Technology CO. LTD
/*    2010-2020 Copyright reversed.
/*    @Funtion Name		:  os_sdk_starttimer
/*    @Description		:  stop timer
/*    @Input para		:  os_timer *pTimer
/*	  @Output para		:  none
/*    @Return			:  time counted
/*    @Author			:  lipeng
/*    @Revison History	:
/*		1. Date			: 2020.8.11
/*		   Author		: lipeng
/*		   Modification	: create the function
/********************************************************************/
int os_sdk_stoptimer(os_timer *pTimer);

/******************************************************************
/*    Kunpeng Technology CO. LTD
/*    2010-2020 Copyright reversed.
/*    @Funtion Name		:  os_sdk_starttimer
/*    @Description		:  sleep some ms
/*    @Input para		:  int msecs
/*	  @Output para		:  none
/*    @Return			:  0
/*    @Author			:  lipeng
/*    @Revison History	:
/*		1. Date			: 2020.8.11
/*		   Author		: lipeng
/*		   Modification	: create the function
/********************************************************************/
int os_sdk_sleep(int msecs);

/******************************************************************
/*    Kunpeng Technology CO. LTD
/*    2010-2020 Copyright reversed.
/*    @Funtion Name		:  os_sdk_starttimer
/*    @Description		:  judge is or not exist of input path
/*    @Input para		:  char *path
/*	  @Output para		:  none
/*    @Return			:  1: exist; 0: nonexist
/*    @Author			:  lipeng
/*    @Revison History	:
/*		1. Date			: 2020.8.11
/*		   Author		: lipeng
/*		   Modification	: create the function
/********************************************************************/
int is_file_exist(char *path);


#ifdef __cplusplus
};
#endif

#endif
