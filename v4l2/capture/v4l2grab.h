/****************************************************/
/*                                                  */
/*                      v4lgrab.h                   */
/*                                                  */
/****************************************************/

#ifndef __V4LGRAB_H__
#define __V4LGRAB_H__


#include <stdio.h>

#define  TRUE	1
#define  FALSE	0

 

#define  FREE(x)       if((x)){free((x));(x)=NULL;}

typedef unsigned char    BYTE;
typedef unsigned short	WORD;
typedef unsigned int    DWORD;

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

 

//if successful return 1,or else return 0
int openVideo();
int closeVideo();

//data 用来存储数据的空间, size空间大小
void getVideoData(unsigned char *data, int size);
 
#if defined(__cplusplus)
}

#endif


#endif //__V4LGRAB_H___
