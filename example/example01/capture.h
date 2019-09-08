/****************************************************/
/*                                                  */
/*                      v4lgrab.h                   */
/*                                                  */
/****************************************************/

#ifndef __V4LGRAB_H__
#define __V4LGRAB_H__


#include <stdio.h>
#include "common.h"


#define  FREE(x)       if((x)){free((x));(x)=NULL;}




struct buffer
{
	void * start;
	unsigned int length;
};




/**/
#pragma pack(1)

typedef struct tagBITMAPFILEHEADER
{
     WORD	    bfType;                // the flag of bmp, value is "BM"
     DWORD    bfSize;                // size BMP file ,unit is bytes
     WORD     bfReserved1;            // 0
     WORD     bfReserved2;            // 0
     DWORD    bfOffBits;             // must be 54

}BITMAPFILEHEADER;



 
typedef struct tagBITMAPINFOHEADER
{
     DWORD    biSize;              // must be 0x28
     DWORD    biWidth;             // picture width
     DWORD    biHeight;            // picture height
     WORD     biPlanes;            // must be 1
     WORD	    biBitCount;          // 1 4 8 16 24 32
     DWORD    biCompression;       //
     DWORD    biSizeImage;         //
     DWORD    biXPelsPerMeter;     //
     DWORD    biYPelsPerMeter;     //
     DWORD    biClrUsed;           //
     DWORD    biClrImportant;      //
}BITMAPINFOHEADER;

typedef struct tagRGBQUAD{
     BYTE	rgbBlue;
     BYTE	rgbGreen;
     BYTE	rgbRed;
     BYTE	rgbReserved;
}RGBQUAD;


#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

 
int v4l2_grab(void);
int init_v4l2(void);
int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);
int close_v4l2(void);
void InitBMPFile(BITMAPFILEHEADER &bf, BITMAPINFOHEADER &bi);

#if defined(__cplusplus)
}

#endif


#endif //__V4LGRAB_H___
