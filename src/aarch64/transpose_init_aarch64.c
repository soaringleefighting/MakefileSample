
void x264_lowres_transpose_neon(unsigned char *dst, unsigned char* src, int width_lowres, int stride_lowres ,int lines_lowres);

/*****************************************************************************
 函 数 名  : x264_lowres_transpose_init
 功能描述  : 将小分辨率顺时针转置90°，用于横向滚动，neon汇编初始化函数
 输入参数  : x264_frame_t *frame  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2020年6月5日
    作    者   : huangzx
    修改内容   : 新生成函数

*****************************************************************************/
//transpose_lowres:转置后小分辨率图像：0: org; 1:H
//i_width_lowres: 小分辨率图像的宽度
//i_stride_lowres:小分辨率图像的跨度
//i_lines_lowres:小分辨率图像的高度
//lowres:小分辨率图像：0: org; 1:H
//只处理Y分量、
/*
void x264_lowres_transpose_init_(x264_frame_t *frame)
{
    for (int i = 0; i < 2; i++)
    {
        pixel *dst, *src;
		int width_lowres, stride_lowres, lines_lowres;
        dst = frame->transpose_lowres[i];
		src = frame->lowres[i];
		width_lowres = frame->i_width_lowres;
		stride_lowres = frame->i_stride_lowres;
		lines_lowres = frame->i_lines_lowres;
		//for (int y = 0; y < frame->i_width_lowres; y++, dst += frame->i_stride_lowres) // width
        //    for (int x = 0; x < frame->i_lines_lowres; x++) // height
        //        dst[x] = frame->lowres[i][(frame->i_lines_lowres - x -1) * frame->i_stride_lowres + y];
		x264_lowres_transpose_neon(dst, src, width_lowres, stride_lowres, lines_lowres);
    }
}
*/


