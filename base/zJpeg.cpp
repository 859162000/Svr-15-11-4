/**
 * \file
 * \version  $Id: zJpeg.cpp  $
 * \author  
 * \date 
 * \brief png��ʽ����֤��������
 */


//#include <gd.h>
//#include <gdfontl.h>
//#include <gdfontg.h>
//#include <gdfontmb.h>

#include "zMisc.h"
#include "zJpeg.h"

namespace Zebra
{
	static const char hexchars[] = "0123456789abcdef";

	/**
	 * \brief jpegͼ����֤��������
	 * \param buffer ���ɵ���֤�ַ���
	 * \param buffer_len ��������֤�ַ������峤��
	 * \param size ������ͼ�����ݵĳ���
	 * \return ������jpegͼ�������ڴ��ַ
	 */
	void *jpegPassport(char *buffer, const int buffer_len, int *size)
	{
//		int i;

		//����ͼƬ
//		gdImagePtr im;
//		im = gdImageCreate(100, 20);
//		if (im)
//		{
//			int white = gdImageColorAllocate(im, 0xff, 0xdf, 0xdf);  
//			int confuseColor = gdImageColorAllocate(im, 0xe9, 0x00, 0x3b);  
//			int textColor = gdImageColorAllocate(im, 0xe9, 0x0e, 0x5b);  
//
//			gdImageFill(im, 0, 0, white);
//			for(i = 0; i < 10; i++)
//			{
//				int cx = zMisc::randBetween(5, 10 + (buffer_len - 1) * 16);
//				int cy = zMisc::randBetween(2, 16);
//				gdImageSetPixel(im, cx, cy, confuseColor);
//			}

//			//������֤��
//			for(i = 0; i < buffer_len - 1; i++)
//			{
//				buffer[i] = hexchars[zMisc::randBetween(0, strlen(hexchars) - 1)];
//				gdImageChar(im,
						/*zMisc::randBetween(0, 1) ? gdFontGetMediumBold() : gdFontGetGiant()*///gdFontGetLarge(),
//						zMisc::randBetween(5, 10) + i * 16,
//						zMisc::randBetween(0, 5),
//						buffer[i],
//						textColor);
//			}
//			buffer[i] = '\0';

			/* Write JPEG using default quality */
//			void *ret = gdImageJpegPtr(im, size, zMisc::randBetween(85, 95));

//			gdImageDestroy(im);

//			return ret;
//		}
//		else
			return NULL;
	}
};
