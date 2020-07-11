#include "../include/libavsample.h"


void libav_init()
{
  transpose_init();
  return RET_OK;
}


void libav_process(unsigned char *dst, unsigned char *src, int width_lowres,
					int stride_lowres, int lines_lowres)
{
  transpose(dst, src, width_lowres, stride_lowres, lines_lowres);
  return RET_OK;
}

int  libav_uninit()
{
  return RET_OK;
}

#endif /*LIBAVSAMPLE_H*/
