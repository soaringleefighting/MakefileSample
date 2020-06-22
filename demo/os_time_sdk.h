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

int os_sdk_inittimer(os_timer *pTimer);
int os_sdk_starttimer(os_timer *pTimer);
int os_sdk_stoptimer(os_timer *pTimer);
int os_sdk_sleep(int msecs);
int is_file_exist(char *path);


#ifdef __cplusplus
};
#endif

#endif
