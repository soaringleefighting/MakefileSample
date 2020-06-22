#include "stdio.h"
#include "stdlib.h"
#include "os_time_sdk.h"
#include "../src/transpose.h"	

#define  HAS_NEON	   (0)		// 0表示纯C，1表示arm neon优化
#define  X86_ASM	   (0)		// 1:开启x86 assembly  0:不开启x86 assembly

#define ENABLE_CHROMA  (1)		// 1: 使能色度   0:只处理亮度

	
/*************调试打印函数***************/
// dx寄存器打印函数
void cprintf(unsigned char *srcu8)
{
	int i=0;
	char *srcs8 = (char *)srcu8;
	printf("u8:"); 
	for(i=0; i < 16; i++){
		printf("%d ", srcu8[i]);
	}
	printf("s8:"); 
	for(i=0; i < 16; i++){
		printf("%d ", srcs8[i]);
	}
	printf("\n");
}

// rx/wx寄存器打印函数
void print(int in0, int in1, int in2, int in3)
{
	printf("\ndec: %d %d %d %d \t", in0, in1, in2, in3);
	printf("hex: %08x %08x %08x %08x\n",in0, in1, in2, in3);
	printf("\n");	
}


/****************主函数入口**********************************/
int main(int argc, char* argv[])
{
	unsigned int width = 0, height = 0, stride = 0;
	unsigned int nw = 0, nh = 0, x = 0, y = 0;
	unsigned int frame_size_y = 0;
	//unsigned int frame_size,
	FILE *fin = NULL, *fou = NULL;
	unsigned char *src = NULL, *dst = NULL;
#if ENABLE_CHROMA
	unsigned char *src_uv = NULL, *dst_uv  = NULL;
#endif
	int frame_num = 0, framenum = 0;
	char outname[512] = { 0, };
	
	os_timer  t_os_timer = {0};
	double  time_count = 0.0, time_count_c = 0.0;
	double  time_avg = 0.0, time_avg_c = 0.0;
	//int i = 0;
	
	if (argc != 6)
	{
		printf("\nUsage:demo.exe input.yuv output.yuv width height framenum.\n\n");
		return -1;
	}
	fin = fopen(argv[1], "rb");
	if (fin == NULL)
	{
		printf("error:open %s fail\n", argv[1]);
		return -1;
	}

	sprintf(outname,"%s", argv[2]);

	width = atoi(argv[3]);
	height = atoi(argv[4]);
	framenum = atoi(argv[5]);
	
	stride = (width + 15) & ~15;  //对齐到16边界
	//height = (height + 15) & ~15;
	//frame_size = width*height * 3 / 2;
	frame_size_y = width * height;
	nw = width / 16;
	nh = height / 16;

	src = (unsigned char *)malloc(frame_size_y);
	if (src == NULL)
	{
		printf("malloc src fail\n");
		return -1;
	}

	dst = (unsigned char *)malloc(frame_size_y);
	if (dst == NULL)
	{
		printf("malloc dst fail\n");
		return -1;
	}
	
#if ENABLE_CHROMA
	src_uv = (unsigned char *)malloc(frame_size_y/4);
	if (src_uv == NULL)
	{
		printf("malloc src_uv fail\n");
		return -1;
	}

	dst_uv = (unsigned char *)malloc(frame_size_y/4);
	if (dst_uv == NULL)
	{
		printf("malloc dst_uv fail\n");
		return -1;
	}
#endif

	fou = fopen(outname, "wb");
	if (fou == NULL)
	{
		printf("error: open %s fail\n", outname);
		return -1;
	}


	os_sdk_inittimer(&t_os_timer);

	while (fread(src, 1, frame_size_y, fin) == frame_size_y)
	{
		// 亮度分量转置 Y
#if HAS_NEON || X86_ASM
#if  HAS_NEON
		printf("arm neon test ...");
		os_sdk_starttimer(&t_os_timer);
		x264_lowres_transpose_neon(dst, src, width, width, height);
		time_count = os_sdk_stoptimer(&t_os_timer);

		os_sdk_starttimer(&t_os_timer);
		libavsample_transpose_c(dst, src, width, width, height);
		time_count_c = os_sdk_stoptimer(&t_os_timer);

#endif


#if X86_ASM
		printf("x86 sse2 test ...");
		os_sdk_starttimer(&t_os_timer);

		ff_x264_lowres_transpose_sse2(dst, src, width, width, height);
		// 下边界的处理
		for (y = nh*16; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				dst[x*height+(height-y-1)] = src[y*width+x];
			}
		}

		//右边界的处理
		for (y = 0; y < height; ++y)
		{
			for (x = nw*16; x < width; ++x)
			{
				dst[x*height+(height-y-1)] = src[y*width+x];
			}
		}

		time_count = os_sdk_stoptimer(&t_os_timer);

		os_sdk_starttimer(&t_os_timer);
		libavsample_transpose_c(dst, src, width, width, height);
		time_count_c = os_sdk_stoptimer(&t_os_timer);
#endif
#else
		os_sdk_starttimer(&t_os_timer);
		libavsample_transpose_c(dst, src, width, width, height);
		time_count_c = os_sdk_stoptimer(&t_os_timer);
#endif

		time_avg += time_count;
		time_avg_c += time_count_c;


		fwrite(dst, 1, frame_size_y, fou);

#if ENABLE_CHROMA
		// 色度分量转置
		// U
		fread(src_uv, 1, frame_size_y/4, fin);
		libavsample_transpose_c(dst_uv, src_uv, width/2, width/2 ,height/2);
		fwrite(dst_uv, 1, frame_size_y/4, fou);
		//V
		fread(src_uv, 1, frame_size_y/4, fin);
		libavsample_transpose_c(dst_uv, src_uv, width/2, width/2 ,height/2);
		fwrite(dst_uv, 1, frame_size_y/4, fou);
#else
		fseek(fin, frame_size_y/2, SEEK_CUR); // 跳过色度分量的处理
#endif

		printf("%dth frame ok!! purec consumed time: %f ms, sse2/neon consumed time: %f ms\n", frame_num, time_count_c, time_count);
		frame_num++;
		
		if(frame_num >= framenum)
			break;
	}

	time_avg_c = time_avg_c/frame_num;
	time_avg = time_avg/frame_num;
	printf("%d frames completed!! purec average time: %f ms, sse2/neon consumed time: %f ms\n", frame_num, time_avg_c, time_avg);
	
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
	
  	return 0;
}