/******************************************************************
/*    Kunpeng Technology CO. LTD
/*    2010-2020 Copyright reversed
/*    @File			:	os_time_sdk.c
/*    @Description	:	this file is function for time sdk. 
/*    @Author		:	lipeng
/*    @Modified		:	2020.8.16	Created
/********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "os_time_sdk.h"


#if defined(WIN32) || defined(UNDER_CE) || defined(WIN64)
int os_sdk_inittimer(os_timer *pTimer)
{
   QueryPerformanceFrequency(&pTimer->lpFrequency);
   return 0;
}

int os_sdk_starttimer(os_timer *pTimer)
{
   QueryPerformanceCounter(&pTimer->tStart);
   return 0;
}

int os_sdk_stoptimer(os_timer *pTimer)
{
   QueryPerformanceCounter(&pTimer->tEnd);
   return (int)((pTimer->tEnd.QuadPart - pTimer->tStart.QuadPart)*1000/(pTimer->lpFrequency.QuadPart));
}
int os_sdk_sleep(int msecs)
{
   Sleep(msecs);
   return 0;
}
int is_file_exist(char *path)
{
   return (_access(path, 0) == -1)? 0 : 1;
}

#elif defined(__GNUC__)
int os_sdk_inittimer(os_timer *pTimer)
{
   pTimer = NULL;
   return 0;
} 
int os_sdk_starttimer(os_timer *pTimer)
{
   gettimeofday(&pTimer->tStart, NULL);
   return 0;
}
int os_sdk_stoptimer(os_timer *pTimer)
{
  gettimeofday(&pTimer->tEnd, NULL);
  return (pTimer->tEnd.tv_sec - pTimer->tStart.tv_sec)*1000 + (pTimer->tEnd.tv_usec - pTimer->tStart.tv_usec)/1000;
}
int os_sdk_sleep(int msecs)
{
  usleep(msecs*1000);
  return 0;
}
int is_file_exist(char *path)
{
  return (access(path , 0) == -1) ? 0 : -1;
}
#endif
