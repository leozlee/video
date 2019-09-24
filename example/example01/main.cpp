#include "capture.h"
#include "display.h"
#include "common.h"
#include <unistd.h>

#define BMPFile    "./image_bmp.bmp"

unsigned char frame_buffer[IMAGEWIDTH * IMAGEHEIGHT * 3];
extern struct buffer* buffers;

extern SDL_Window* gWindow;
extern SDL_Surface* gScreenSurface;
extern SDL_Surface* gHelloWorld;


int main(void)
{
    FILE* fp1;
    int grab;

    BITMAPFILEHEADER   bf;
    BITMAPINFOHEADER   bi;

	//due to the camera is not ready, some of the begin frames are not good,
	//maybe we should skip the frame
    fp1 = fopen(BMPFile, "wb");
    if(!fp1)
	{
		printf("open bmp file fail\r\n");
		return(FALSE);
	}
	
	if(init_v4l2() == FALSE) 
	{
     	return(FALSE);
	}

	//Start up SDL and create window
	if(!sdl_init())
	{
		printf( "Failed to initialize!\n" );
        return(FALSE);
	}

    InitBMPFile(bf, bi);
    grab = v4l2_grab();
	convert_yuv_to_rgb_buffer((unsigned char *)buffers[0].start, frame_buffer, IMAGEWIDTH, IMAGEHEIGHT);
    fwrite(&bf, 14, 1, fp1);
    fwrite(&bi, 40, 1, fp1);    
    fwrite(frame_buffer, bi.biSizeImage, 1, fp1);
    fclose(fp1);
    close_v4l2();

    //Load media
    if(!loadMedia(BMPFile))
    {
        printf( "Failed to load media!\n" );
        return(FALSE);
    }

    //Apply the image
    SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );
    
    //Update the surface
    SDL_UpdateWindowSurface(gWindow);

    //Wait two seconds
    //SDL_Delay( 2000 );
    pause();
    
	//Free resources and close SDL
	sdl_close();

    return(TRUE);
}
