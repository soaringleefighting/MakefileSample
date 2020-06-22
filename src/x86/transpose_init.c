//#include "../../utils/x86/cpu.h"
#include "../transpose.h"

void ff_x264_lowres_transpose_sse2(unsigned char *dst, unsigned char* src, int width_lowres, int stride_lowres ,int lines_lowres);
void ff_x264_lowres_transpose_avx2(unsigned char *dst, unsigned char* src, int width_lowres, int stride_lowres ,int lines_lowres);

void transpose_init_x86(unsigned char *dst, unsigned char* src, int width_lowres, int stride_lowres ,int lines_lowres)
{
	//int cpu_flags = av_get_cpu_flags();

	//if (EXTERNAL_SSE2(cpu_flags))
	{
<<<<<<< HEAD
		//printf("cpu feature sse2!!!\n");
		ff_x264_lowres_transpose_sse2(dst, src, width_lowres, stride_lowres, lines_lowres);
	}

#if ARCH_X86_64
	//if (EXTERNAL_AVX2(cpu_flags))
	{
		//printf("cpu feature avx2!!!\n");
		ff_x264_lowres_transpose_avx2(dst, src, width_lowres, stride_lowres, lines_lowres);
	}
#endif
=======
		printf("cpu feature sse2!!!\n");
		ff_x264_lowres_transpose_sse2(dst, src, width_lowres, stride_lowres, lines_lowres);
	}

//#if ARCH_X86_64
	//if (EXTERNAL_AVX2(cpu_flags))
	{
		ff_x264_lowres_transpose_avx2(dst, src, width_lowres, stride_lowres, lines_lowres);
		printf("cpu feature avx2!!!\n");
	}
//#endif
>>>>>>> f44ab05a867ce7c72e5be5bdb603ef736774740a
}
