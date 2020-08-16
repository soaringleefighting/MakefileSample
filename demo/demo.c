/******************************************************************
/*    Kunpeng Technology CO. LTD
/*    2010-2020 Copyright reversed.
/*    @File				:	demo.c
/*    @Description		:   this file is test demo for libavsample library. 
/*	  @Feature			:	1.支持跨平台时间统计功能;
/*							2.支持两种命令行参数解析方式;
/*							3.支持Git版本获取;
/*							4.支持linux下进程cpu核心绑定;
/*							5.支持demo中多线程测试
/*							6.支持内存检查工具CRTDBG
/*    @Author			:	lipeng		
/*    @Revison History	:
/*		1. Date			: 2020.8.1
/*		   Author		: lipeng
/*		   Modification	: create the file
/*		2. Date			: 2020.8.10
/*		   Author		: lipeng
/*		   Modification	: support cpu core affinity.	
/*		3. Date			: 2020.8.16
/*		   Author		: lipeng
/*		   Modification	: support multiple thread test and memory check.			
/*	  @Version	: 1.0.2
/********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>		//TODO: 类型定义统一为define.h

#include "os_time_sdk.h"
#include "libavsample.h"

#if defined(__GNUC__) 
#include <unistd.h>
#include <getopt.h>          /* getopt_long所在头文件 */

#if CONFIG_CORE
//#define _GNU_SOURCE			 /* 启动CPU_ZERO和CPU_SET等系统函数 */
#define __USE_GNU
#include <sched.h>			 /* sched_setaffinity和sched_getaffinity所在头文件 *///
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#endif
#endif  /* End of #if defined(__GNUC__) */

#define  HAS_NEON			(0)		// 0表示纯C，1表示arm neon优化
#define  X86_ASM			(0)		// 1:开启x86 assembly  0:不开启x86 assembly

#define ENABLE_CHROMA		(1)		// 1: 使能色度   0:只处理亮度

// 注意:OPTION_PARSE_BASE和OPTION_PARSE_LINUX只能打开一个，且OPTION_PARSE_LINUX只能用于LINUX平台下
// 两个宏都设置为1时，走getopt分支。
#define OPTION_PARSE_BASE   (1) // 简单的命令行参数解析方式，使用strncmp
// ./demo -i BlowingBubbles_416x240_50.yuv -o  out_sse2.yuv  -wt 416  -ht 240  -fr 100

#define OPTION_PARSE_LINUX	(0)	// LINUX下的命令行参数解析方式，使用getopt
// 长选项: ./demo  --input SlideEditing_1280x720_30.yuv --output out.yuv --uiWidth 1280 --uiHeight 720 --framenum 100
// 短选项: ./demo  -i SlideEditing_1280x720_30.yuv -o out.yuv -w 1280 -t 720 -n 10

#define MULTI_THREAD		(0)

#if MULTI_THREAD
#include "pthread.h"
#endif

/*内存检测工具CRTDBG*/
#ifdef _MSC_VER && _DEBUG
#define MEM_CK 	(1)
#else
#define	MEM_CK	(0)
#endif

#if MEM_CK
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

/*************调试打印函数***************/
/* dx寄存器打印函数 */
void cprintf(unsigned char *srcu8)
{
	int i=0;
	char *srcs8 = (char *)srcu8;
	printf("u8:"); 
	for(i=0; i < 16; i++)
	{
		printf("%d ", srcu8[i]);
	}
	printf("s8:"); 
	for(i=0; i < 16; i++)
	{
		printf("%d ", srcs8[i]);
	}
	printf("\n");
}

/* rx/wx寄存器打印函数 */
void print(int in0, int in1, int in2, int in3)
{
	printf("\ndec: %d %d %d %d \t", in0, in1, in2, in3);
	printf("hex: %08x %08x %08x %08x\n",in0, in1, in2, in3);
	printf("\n");	
}

#if OPTION_PARSE_BASE
/* 打印帮助函数 */
int print_help(char* str)
{
	printf("%s \nUsage: \n", str);
	printf(" -help : this help \n");
	printf(" -i    : input yuv file \n");
	printf(" -o    : output yuv file \n");
	printf(" -wt   : uiWidth \n");
	printf(" -ht   : uiHeight \n");
	printf(" -fr   : framenum \n");
	printf("Example: ./demo -i BlowingBubbles_416x240_50.yuv -o  out_sse2.yuv  -wt 416  -ht 240  -fr 100\n");
	return 0;
}
#endif

#if OPTION_PARSE_LINUX
static const char *short_options = "i:o:w:t:n:hv";
// struct option 
// {  
// 	const char *name;     //表示选项的名称
// 	int         has_arg;  //表示选项后面是否携带参数。该参数有三个不同值：
// 	//no_argument(或者是0)；required_argument(或者是1)；optional_argument(或者是2)
// 	int        *flag;   // 空或者非空。
// 	//a:如果参数为空NULL，那么当选中某个长选项的时候，getopt_long将返回val值。
// 	// b:如果参数不为空，那么当选中某个长选项的时候，getopt_long将返回0，并且将flag指针参数指向val值。
// 	int         val;   //表示指定函数找到该选项时的返回值，或者当flag非空时指定flag指向的数据的值val
// }; 
static const struct option long_options[] = 
{
	{ "input", required_argument, NULL, 'i' },
	{ "output", required_argument, NULL, 'o' },
	{ "uiWidth", required_argument, NULL, 'w' },
	{ "uiHeight", required_argument, NULL, 't' },
	{ "framenum", required_argument, NULL, 'n' },
	{ "version", no_argument, NULL, 'v' },
	{ "help",	no_argument, NULL, 'h' },
	{ 0, 0, 0, 0 }
};

/* 打印帮助函数 */
int print_help_getopt(char *str)
{
	printf("%s \nUsage: \n", str);
	printf(" --help(-h)		: this help \n");
	printf(" --input(-i)    : input yuv file \n");
	printf(" --output(-o)   : output yuv file \n");
	printf(" --uiWidth(-w)	: uiWidth \n");
	printf(" --uiHeight(-h)   : uiHeight \n");
	printf(" --framenum(-f) : framenum \n");
	printf("Example: ./demo  -i SlideEditing_1280x720_30.yuv -o out.yuv -w 1280 -t 720 -n 10\n");
	return 0;
}
#endif

#if CONFIG_CORE  /* CPU进程绑定：指定当前运行的CPU核心*/
static void setAffinity_CPU(pid_t tid, int coreindex)
{
	cpu_set_t cs;
	CPU_ZERO(&cs);
	CPU_SET(coreindex, &cs);
	sched_setaffinity(tid, sizeof(cs), &cs);
} 
#endif

#if MULTI_THREAD
#define MAX_THREAD_NUM		(64)
/* 多线程传递参数 */
typedef struct stArg 
{
	char* input_str;	/* input  */
	char* output_str;	/* output */
	int	  uiWidth;		/* uiWidth  */
	int   uiHeight;		/* uiHeight */
	int   framenum;		/* framenum	  */
	int	  thread_num;	/* thread_num */
}stArg_pthread;

/****************主函数入口**********************************/
void* alg_main(void* arg)
 {
 	unsigned int uiWidth			= 0;
 	unsigned int uiHeight			= 0;
 	unsigned int uiFramesize	= 0;
 	FILE *fin					= NULL;
 	FILE *fou					= NULL;
 	unsigned char *src			= NULL;
 	unsigned char *dst			= NULL;
 
 #if ENABLE_CHROMA
 	unsigned char *src_uv		= NULL;
 	unsigned char *dst_uv		= NULL;
 #endif
 	int frame_num				= 0;
 	int framenum				= 50;  /* 默认50帧 */
 
 	/* 支持跨平台时间统计 */
 	os_timer  t_os_timer		= {0};
 	double  time_count			= 0.0;
 	double	time_count_c		= 0.0;
 	double  time_avg			= 0.0;
 	double  time_avg_c			= 0.0;
 
 	stArg_pthread* args		= (stArg_pthread*) arg;
 	//*******1.支持版本获取*********/
 	printf("[Current library info] %s.\n", libav_getversion());
 
 	fin		= fopen(args->input_str, "rb");
 	if ( NULL == fin )
 	{
 		printf("[demo] error:open %s fail\n", args->input_str);
 		return RET_FAIL;
 	}
 
 	fou     = fopen(args->output_str, "wb");
 	if ( NULL == fou )
 	{
 		printf("[demo] error:open %s fail\n", args->output_str);
 		return RET_FAIL;
 	}
 
 	uiWidth			= args->uiWidth;
 	uiHeight			= args->uiHeight;
 	framenum		= args->framenum;
 
 	uiFramesize	= uiWidth * uiHeight;
 
 	//*******2.输入图像和输出图像内存分配*********/
 	src = (unsigned char *)malloc(uiFramesize);
 	if ( NULL == src )
 	{
 		printf("[demo] malloc src fail\n");
 		return RET_MALLOC;
 	}
 
 	dst = (unsigned char *)malloc(uiFramesize);
 	if ( NULL == dst )
 	{
 		printf("[demo] malloc dst fail\n");
 		return RET_MALLOC;
 	}
 
 #if ENABLE_CHROMA
 	src_uv = (unsigned char *)malloc(uiFramesize/4);
 	if ( NULL == src_uv )
 	{
 		printf("[demo] malloc src_uv fail\n");
 		return RET_MALLOC;
 	}
 
 	dst_uv = (unsigned char *)malloc(uiFramesize/4);
 	if ( NULL == dst_uv )
 	{
 		printf("[demo] malloc dst_uv fail\n");
 		return RET_MALLOC;
 	}
 #endif
 
 	//*******3.函数指针初始化*********/
 	libav_init();
 
 	os_sdk_inittimer(&t_os_timer);
 
 	while ( fread(src, 1, uiFramesize, fin) == uiFramesize )
 	{
 		//*******4.主处理*********/
 		// 亮度分量转置 Y
 #if HAS_NEON || X86_ASM
 #if  HAS_NEON
 		printf("[demo] arm neon test ...");
 		os_sdk_starttimer(&t_os_timer);
 		x264_lowres_transpose_neon(dst, src, uiWidth, uiWidth, uiHeight);
 		time_count = os_sdk_stoptimer(&t_os_timer);
 
 		os_sdk_starttimer(&t_os_timer);
 		libav_process(dst, src, uiWidth, uiWidth, uiHeight);
 		time_count_c = os_sdk_stoptimer(&t_os_timer);
 #endif
 
 
 #if X86_ASM
 		printf("[demo] x86 sse2 test ...");
 		os_sdk_starttimer(&t_os_timer);
 
 		ff_x264_lowres_transpose_sse2(dst, src, uiWidth, uiWidth, uiHeight);
 
 		time_count = os_sdk_stoptimer(&t_os_timer);
 
 		os_sdk_starttimer(&t_os_timer);
 		libav_process(dst, src, uiWidth, uiWidth, uiHeight);
 		time_count_c = os_sdk_stoptimer(&t_os_timer);
 #endif
 #else
 		os_sdk_starttimer(&t_os_timer);
 		libav_process(dst, src, uiWidth, uiWidth, uiHeight);
 		time_count_c = os_sdk_stoptimer(&t_os_timer);
 #endif
 
 		time_avg   += time_count;
 		time_avg_c += time_count_c;
 
 		fwrite(dst, 1, uiFramesize, fou);
 
 #if ENABLE_CHROMA
 		// 色度分量转置
 		// U
 		if( uiFramesize/4 != fread(src_uv, 1, uiFramesize/4, fin) )
 		{
 			return RET_FAIL;
 		}
 		libav_process(dst_uv, src_uv, uiWidth/2, uiWidth/2 ,uiHeight/2);
 
 		fwrite(dst_uv, 1, uiFramesize/4, fou);
 		//V
 		if ( uiFramesize/4 != fread(src_uv, 1, uiFramesize/4, fin) )
 		{
 			return RET_FAIL;
 		}
 		libav_process(dst_uv, src_uv, uiWidth/2, uiWidth/2 ,uiHeight/2);
 		fwrite(dst_uv, 1, uiFramesize/4, fou);
 #else
 		fseek(fin, uiFramesize/2, SEEK_CUR); // 跳过色度分量的处理
 #endif
 
 		printf("%dth frame ok!! purec consumed time: %f ms, sse2/neon consumed time: %f ms\n", frame_num, time_count_c, time_count);
 		frame_num++;
 
 		if(frame_num >= framenum)
 			break;
 	}
 
 	time_avg_c	= time_avg_c / frame_num;
 	time_avg	= time_avg   / frame_num;
 	printf("%d frames completed!! purec average time: %f ms, sse2/neon consumed time: %f ms\n", frame_num, time_avg_c, time_avg);
 
 	//*******5.函数指针反初始化*********/
 	libav_uninit();
 
 	free(src);
 	src = NULL;
 	free(dst);
 	dst = NULL;
 #if ENABLE_CHROMA
 	free(src_uv);
 	src_uv = NULL;
 	free(dst_uv);
 	dst_uv = NULL;
 #endif
 
 	fclose(fin);
 	fclose(fou);
  
 	return RET_OK;
 }


int main(int argc, char *argv[])
{
	int	i = 0;
	int thread_num = 0;
	pthread_t t1		= {0};
	stArg_pthread args	= {0};
	//*******1.命令行输入参数解析*********/
	if (argc != 7)
	{
		printf("\nUsage:demo.exe input.yuv output.yuv uiWidth uiHeight framenum thread_num.\n\n");
		return RET_FAIL;
	}

	args.input_str  = argv[1];
	args.output_str = argv[2];
	args.uiWidth      = atoi(argv[3]);
	args.uiHeight     = atoi(argv[4]);
	args.framenum   = atoi(argv[5]);
	thread_num		= atoi(argv[6]);

	args.thread_num = (thread_num > MAX_THREAD_NUM) ? MAX_THREAD_NUM : thread_num;

	for( i = 0; i < args.thread_num; i++ )
	{
		printf("[demo] threadnum: %d\n", i);
		pthread_create(&t1, NULL, alg_main, (void*)&args);
		pthread_join(t1, NULL);
	}

#if _WIN32
	system("pause");
#endif

	return RET_OK;
}

#else
/****************主函数入口**********************************/
int main(int argc, char* argv[])
{
	int			 iRet			 = RET_OK;
	unsigned int uiWidth		 = 0;
	unsigned int uiHeight		 = 0;
	unsigned int uiFramesize	 = 0;
	FILE		  *fin		= NULL;
	FILE		  *fou		= NULL;
	unsigned char *src		= NULL;
	unsigned char *dst		= NULL;
#if ENABLE_CHROMA
	unsigned char *src_uv	= NULL;
	unsigned char *dst_uv	= NULL;
#endif
	int frame_num			= 0;
	int	framenum			= 50;  /* 默认50帧 */
	
#if CONFIG_CORE
	int coreindex			= 0;
#endif

	/* 支持跨平台时间统计 */
	os_timer  t_os_timer	= {0};
	double    time_count	= 0.0;
	double	  time_count_c	= 0.0;
	double    time_avg		= 0.0;
	double	  time_avg_c	= 0.0;

//*******1.命令行输入参数解析*********/
#if !OPTION_PARSE_LINUX	
#if OPTION_PARSE_BASE
	/* 采用strncmp实现命令行参数解析 */
	int i = 0;
	char *input_str	 = NULL;
	char *output_str = NULL;
	if (argc >= 11)
	{
	    i= 0;
		for (i = 1; i < argc; ++i)
		{
			if (!strncmp(argv[i], "-help", 5))  // help
			{
				print_help(argv[0]);
			}
			else if (!strncmp(argv[i], "-i", 2)) // input
			{
				input_str = argv[++i];
			}
			else if (!strncmp(argv[i], "-o", 2)) // output
			{
				output_str = argv[++i];
			}
			else if (!strncmp(argv[i], "-wt", 3)) // uiWidth
			{
				uiWidth = atoi(argv[++i]);
			}
			else if (!strncmp(argv[i], "-ht", 3)) // uiHeight
			{
				uiHeight = atoi(argv[++i]);
			}
			else if (!strncmp(argv[i], "-fr", 3)) // framenum
			{
				framenum = atoi(argv[++i]);
			}
		}
	}
	else
	{
		print_help(argv[0]);
		return RET_FAIL;
	}

	fin = fopen(input_str, "rb");
	if ( NULL == fin )
	{
		printf("[demo] error:open %s fail\n", input_str);
		return RET_FAIL;
	}

	fou = fopen(output_str, "wb");
	if ( NULL == fou )
	{
		printf("[demo] error: open %s fail\n", output_str);
		return RET_FAIL;
	}

#else
	char outname[512] = { 0, };

	if (argc != 6)
	{
		printf("\nUsage:demo.exe input.yuv output.yuv uiWidth uiHeight framenum.\n\n");
		return RET_FAIL;
	}

	fin = fopen(argv[1], "rb");
	if ( NULL == fin )
	{
		printf("[demo] error:open %s fail\n", argv[1]);
		return RET_FAIL;
	}

	sprintf(outname,"%s", argv[2]);

	uiWidth = atoi(argv[3]);
	uiHeight = atoi(argv[4]);
	framenum = atoi(argv[5]);

	fou = fopen(outname, "wb");
	if ( NULL == fou )
	{
		printf("[demo] error: open %s fail\n", outname);
		return RET_FAIL;
	}
#endif
#endif

#if defined(__GNUC__)  /* 针对LINUX平台 */
#if OPTION_PARSE_LINUX
	// 采用getopt和getopt_long实现命令行参数解析
	char input_str[100] = {0};
	char output_str[100]= {0};

	int option_index	= 0;
	//int opterr = 0;
	int ch				= 0;
	if (argc > 1)
	{
		while ((ch = getopt_long(argc, argv, short_options, long_options,
			&option_index)) != -1) {
				switch (ch) {
				case 'h':
					print_help_getopt(argv[0]);
					return -1;
				case 'i':
					strcpy(input_str, optarg);
					break;
				case 'o':
					strcpy(output_str, optarg);
					break;
				case 'w':
					uiWidth = atoi(optarg);
					break;
				case 't':
					uiHeight = atoi(optarg);
					break;
				case 'n':
					framenum = atoi(optarg);
					break;
					//case 'v':
					//	verbose = 1;
					//	break;
				default:
					printf("[libAVSample] Demo: unknown option found: %c\n", ch);
					return RET_FAIL;
				}
		}
	}
	else
	{
		print_help_getopt(argv[0]);
		return RET_FAIL;
	}


	fin = fopen(input_str, "rb");
	if ( NULL == fin )
	{
		printf("[demo] error:open %s fail\n", input_str);
		return RET_FAIL;
	}

	fou = fopen(output_str, "wb");
	if ( NULL == fou )
	{
		printf("[demo] error: open %s fail\n", output_str);
		return RET_FAIL;
	}
#endif

#if CONFIG_CORE
	coreindex = 3;
	pid_t tid = 0;
	
	tid = syscall(SYS_gettid);  /* 获取当前进程pid */
	setAffinity_CPU(tid, coreindex);  /* 指定程序在CPU为3的核心上运行，不指定时，运行的CPU随机分配 */
	printf("[demo] current process run on cpu %d\n", coreindex);
#endif

#endif  /* End of #if defined(__GNUC__)*/
	
	//下述代码放在demo的开始或中间位置
	/* 内存检查工具CRTDBG*/
#if MEM_CK
	//_CrtSetBreakAlloc(63); //注：这里的63表示内存分配的次数，其值为调试信息的{}中的值。
#endif

#if MEM_CK
	_CrtCheckMemory();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	/* 支持版本获取 */
	printf("[Current library info] %s.\n", libav_getversion());

	uiFramesize = uiWidth * uiHeight;

	//*******2.输入图像和输出图像内存分配*********/
	src = (unsigned char *)malloc(uiFramesize);
	if ( NULL == src )
	{
		printf("[demo] malloc src fail\n");
		iRet =  RET_MALLOC;
		goto FAIL;
	}

	dst = (unsigned char *)malloc(uiFramesize);
	if ( NULL == dst )
	{
		printf("[demo] malloc dst fail\n");
		iRet =  RET_MALLOC;
		goto FAIL;
	}
	
#if ENABLE_CHROMA
	src_uv = (unsigned char *)malloc(uiFramesize/4);
	if ( NULL == src_uv )
	{
		printf("[demo] malloc src_uv fail\n");
		iRet =  RET_MALLOC;
		goto FAIL;
	}

	dst_uv = (unsigned char *)malloc(uiFramesize/4);
	if ( NULL == dst_uv )
	{
		printf("[demo] malloc dst_uv fail\n");
		iRet =  RET_MALLOC;
		goto FAIL;
	}
#endif

//*******3.函数指针初始化*********/
	libav_init();

	os_sdk_inittimer(&t_os_timer);

	while (fread(src, 1, uiFramesize, fin) == uiFramesize)
	{
		//*******4.主处理*********/
		// 亮度分量转置 Y
#if HAS_NEON || X86_ASM
#if  HAS_NEON
		printf("arm neon test ...");
		os_sdk_starttimer(&t_os_timer);
		x264_lowres_transpose_neon(dst, src, uiWidth, uiWidth, uiHeight);
		time_count = os_sdk_stoptimer(&t_os_timer);

		os_sdk_starttimer(&t_os_timer);
		libav_process(dst, src, uiWidth, uiWidth, uiHeight);
		time_count_c = os_sdk_stoptimer(&t_os_timer);
#endif


#if X86_ASM
		printf("x86 sse2 test ...");
		os_sdk_starttimer(&t_os_timer);

		ff_x264_lowres_transpose_sse2(dst, src, uiWidth, uiWidth, uiHeight);

		time_count = os_sdk_stoptimer(&t_os_timer);

		os_sdk_starttimer(&t_os_timer);
		libav_process(dst, src, uiWidth, uiWidth, uiHeight);
		time_count_c = os_sdk_stoptimer(&t_os_timer);
#endif
#else
		os_sdk_starttimer(&t_os_timer);
		libav_process(dst, src, uiWidth, uiWidth, uiHeight);
		time_count_c = os_sdk_stoptimer(&t_os_timer);
#endif

		time_avg   += time_count;
		time_avg_c += time_count_c;


		fwrite(dst, 1, uiFramesize, fou);

#if ENABLE_CHROMA
		// 色度分量转置
		// U
		if(uiFramesize/4 != fread(src_uv, 1, uiFramesize/4, fin))
		{
			iRet =  RET_FAIL;
			goto FAIL;
		}
		libav_process(dst_uv, src_uv, uiWidth/2, uiWidth/2 ,uiHeight/2);

		fwrite(dst_uv, 1, uiFramesize/4, fou);
		//V
		if (uiFramesize/4 != fread(src_uv, 1, uiFramesize/4, fin))
		{
			iRet =  RET_FAIL;
			goto FAIL;
		}
		libav_process(dst_uv, src_uv, uiWidth/2, uiWidth/2 ,uiHeight/2);
		fwrite(dst_uv, 1, uiFramesize/4, fou);
#else
		fseek(fin, uiFramesize/2, SEEK_CUR); // 跳过色度分量的处理
#endif

		printf("%dth frame ok!! purec consumed time: %f ms, sse2/neon consumed time: %f ms\n", frame_num, time_count_c, time_count);
		frame_num++;
		
		if(frame_num >= framenum)
			break;
	}

	time_avg_c = time_avg_c/frame_num;
	time_avg   = time_avg/frame_num;
	printf("%d frames completed!! purec average time: %f ms, sse2/neon consumed time: %f ms\n", frame_num, time_avg_c, time_avg);
	
	//*******5.函数指针反初始化*********/
	libav_uninit();

FAIL:
	if ( NULL != src )
	{
		free(src);
		src = NULL;
	}

	if ( NULL != dst )
	{
		free(dst);
		dst = NULL;
	}

#if ENABLE_CHROMA
	if ( NULL != src_uv )
	{
		free(src_uv);
		src_uv = NULL;
	}

	if (NULL != dst_uv )
	{
		free(dst_uv);
		dst_uv = NULL;
	}
#endif

	if ( NULL != fin )
	{
		fclose(fin);
	}

	if ( NULL != fou )
	{
		fclose(fou);
	}

#if _WIN32
	system("pause");
#endif
	
  	return RET_OK;
}
#endif