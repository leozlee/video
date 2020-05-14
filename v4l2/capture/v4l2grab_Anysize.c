#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <linux/types.h>
#include <linux/videodev2.h>
#include <jpeglib.h>
#include "v4l2grab.h"
 

#define FILE_VIDEO 			"/dev/video0"
#define BMP      			"./image_bmp.bmp"

#define  IMAGEWIDTH    800
#define  IMAGEHEIGHT   600

static int fd;
static struct v4l2_capability cap;
struct v4l2_fmtdesc fmtdesc;
struct v4l2_format fmt,fmtack;
struct v4l2_streamparm setfps;  
struct v4l2_requestbuffers req;
struct v4l2_buffer buf;
enum v4l2_buf_type type;
unsigned char frame_buffer[IMAGEWIDTH * IMAGEHEIGHT * 3];


struct buffer
{
	void * start;
	unsigned int length;
} * buffers;
 


int init_v4l2(void)
{
	int i;
	int ret = 0;
	
	//opendev
	if ((fd = open(FILE_VIDEO, O_RDWR)) == -1) 
	{
		printf("Error opening V4L interface\n");
		return (FALSE);
	}

	//query cap
	if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) 
	{
		printf("Error opening device %s: unable to query device.\n",FILE_VIDEO);
		return (FALSE);
	}
	else
	{
     	printf("driver:\t\t%s\n", cap.driver);
     	printf("card:\t\t%s\n", cap.card);
     	printf("bus_info:\t%s\n", cap.bus_info);
     	printf("version:\t%d\n", cap.version);
     	printf("capabilities:\t%x\n", cap.capabilities);
     	
     	if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE) 
     	{
			printf("Device %s: supports capture.\n",FILE_VIDEO);
		}

		if ((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING) 
		{
			printf("Device %s: supports streaming.\n",FILE_VIDEO);
		}
	} 
	
	//emu all support fmt
	fmtdesc.index = 0;
	fmtdesc.type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	printf("Support format:\n");
	while(ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc)!=-1)
	{
		printf("\t%d.%s\n",fmtdesc.index+1,fmtdesc.description);
		fmtdesc.index++;
	}
	
    //set fmt
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	fmt.fmt.pix.height      = IMAGEHEIGHT;
	fmt.fmt.pix.width       = IMAGEWIDTH;
	fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
	
	if(ioctl(fd, VIDIOC_S_FMT, &fmt) == -1)
	{
		printf("Unable to set format\n");
		return FALSE;
	} 	

	
	if(ioctl(fd, VIDIOC_G_FMT, &fmt) == -1)
	{
		printf("Unable to get format\n");
		return FALSE;
	} 
	{
     	printf("fmt.type:\t\t%d\n", fmt.type);
     	printf("pix.pixelformat:\t%c%c%c%c\n",
				fmt.fmt.pix.pixelformat & 0xFF, 
				(fmt.fmt.pix.pixelformat >> 8) & 0xFF,
				(fmt.fmt.pix.pixelformat >> 16) & 0xFF, 
				(fmt.fmt.pix.pixelformat >> 24) & 0xFF);

     	printf("pix.height:\t\t%d\n", fmt.fmt.pix.height);
     	printf("pix.width:\t\t%d\n", fmt.fmt.pix.width);
     	printf("pix.field:\t\t%d\n", fmt.fmt.pix.field);
	}
	//set fps
	setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	setfps.parm.capture.timeperframe.numerator   = 10;
	setfps.parm.capture.timeperframe.denominator = 10;
	
	printf("init %s \t[OK]\n",FILE_VIDEO);
	    
	return TRUE;
}

int v4l2_grab(void)
{
	unsigned int n_buffers;
	
	//request for 4 buffers 
	req.count  = 4;
	req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	if(ioctl(fd, VIDIOC_REQBUFS, &req) == -1)
	{
		printf("request for buffers error\n");
		return -1;
	}

	//mmap for buffers
	buffers = malloc(req.count*sizeof (*buffers));
	if (!buffers) 
	{
		printf ("Out of memory\n");
		return(FALSE);
	}
	
	for (n_buffers = 0; n_buffers < req.count; n_buffers++) 
	{
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;
		//query buffers
		if (ioctl (fd, VIDIOC_QUERYBUF, &buf) == -1)
		{
			printf("query buffer error\n");
			return(FALSE);
		}

		buffers[n_buffers].length = buf.length;
		//map
		buffers[n_buffers].start = mmap(NULL,buf.length,
						PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
						
		if (buffers[n_buffers].start == MAP_FAILED)
		{
			printf("buffer map error\n");
			return(FALSE);
		}
	}
		
	//queue
	for (n_buffers = 0; n_buffers < req.count; n_buffers++)
	{
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;
		ioctl(fd, VIDIOC_QBUF, &buf);
	} 
	

	//start
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ioctl (fd, VIDIOC_STREAMON, &type);
	

	//end, because we just need a frame
	ioctl(fd, VIDIOC_DQBUF, &buf);

    printf("grab yuyv OK\r\n");


	return(TRUE);
}



int convert_yuv_to_rgb_pixel(int y, int u, int v)
{
	unsigned int pixel32 = 0;
	unsigned char *pixel = (unsigned char *)&pixel32;
	int r, g, b;

	r = y + (1.370705 * (v-128));
	g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
	b = y + (1.732446 * (u-128));

	if(r > 255) r = 255;
	if(g > 255) g = 255;
	if(b > 255) b = 255;
	if(r < 0) r = 0;
	if(g < 0) g = 0;
	if(b < 0) b = 0;

	pixel[0] = r * 220 / 256;
	pixel[1] = g * 220 / 256;
	pixel[2] = b * 220 / 256;
	return pixel32;
}
 



int convert_yuv_to_rgb_pixel_v2(int y, int u, int v)
{
	unsigned int pixel32 = 0;
	unsigned char *pixel = (unsigned char *)&pixel32;
	int r, g, b;

	r = y + 1.042 * (v-128);
	g = y - 0.34414 *(u-128) - 0.71414 * (v-128);
	b = y + 1.772 * (u-128);

	if(r > 255) r = 255;
	if(g > 255) g = 255;
	if(b > 255) b = 255;
	if(r < 0) r = 0;
	if(g < 0) g = 0;
	if(b < 0) b = 0;

	pixel[0] = r;
	pixel[1] = g;
	pixel[2] = b;
	return pixel32;
}






int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
	unsigned int in, out = 0;
	unsigned int macropixel;
	unsigned char pixel_24[3];
	unsigned int pixel32;
	int y0, u, y1, v;

	for(in = 0; in < width * height * 2; in += 4) 
	{
		macropixel = yuv[in + 3] << 24 | yuv[in + 2] << 16 | yuv[in + 1] <<  8 | yuv[in + 0];
		y0 = (macropixel & 0x000000ff);
		u  = (macropixel & 0x0000ff00) >>  8;
		y1 = (macropixel & 0x00ff0000) >> 16;
		v  = (macropixel & 0xff000000) >> 24;
		pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
		pixel_24[0] = (pixel32 & 0x000000ff);			//R	
		pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;		//G
		pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;		//B

		rgb[out++] = pixel_24[0];
		rgb[out++] = pixel_24[1];
		rgb[out++] = pixel_24[2];
		pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
		pixel_24[0] = (pixel32 & 0x000000ff);
		pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
		pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;

		rgb[out++] = pixel_24[0];
		rgb[out++] = pixel_24[1];
		rgb[out++] = pixel_24[2];
	}
	return 0;
}







int yuyv_2_rgb888(void)
{
	int           i,j;
    unsigned char y1,y2,u,v;
    int r1,g1,b1,r2,g2,b2;
    char * pointer = buffers[0].start;
	
    for(i = 0; i<IMAGEHEIGHT; i++)
    {
    	for(j = 0;j < (IMAGEWIDTH/2); j++)
    	{
    		y1 = *( pointer + (i*(IMAGEWIDTH/2)+j)*4);
    		u  = *( pointer + (i*(IMAGEWIDTH/2)+j)*4 + 1);
    		y2 = *( pointer + (i*(IMAGEWIDTH/2)+j)*4 + 2);
    		v  = *( pointer + (i*(IMAGEWIDTH/2)+j)*4 + 3);
    		
    		r1 = y1 + 1.042 * (v-128);
    		g1 = y1 - 0.34414 *(u-128) - 0.71414 * (v-128);
    		b1 = y1 + 1.772 * (u-128);
    		
    		r2 = y2 + 1.042*(v-128);
    		g2 = y2 - 0.34414*(u-128) - 0.71414 * (v-128);
    		b2 = y2 + 1.772*(u-128);
    		
    		if(r1>255)
    			r1 = 255;
    		else if(r1<0)
    			r1 = 0;
    		
    		if(b1>255)
    			b1 = 255;
    		else if(b1<0)
    			b1 = 0;	
    		
    		if(g1>255)
    			g1 = 255;
    		else if(g1<0)
    			g1 = 0;	
    			
    		if(r2>255)
    			r2 = 255;
    		else if(r2<0)
    			r2 = 0;
    		
    		if(b2>255)
    			b2 = 255;
    		else if(b2<0)
    			b2 = 0;	
    		
    		if(g2>255)
    			g2 = 255;
    		else if(g2<0)
    			g2 = 0;		
    			
    		*(frame_buffer + ((IMAGEHEIGHT-1-i)*(IMAGEWIDTH/2)+j)*6    ) = (unsigned char)b1;
    		*(frame_buffer + ((IMAGEHEIGHT-1-i)*(IMAGEWIDTH/2)+j)*6 + 1) = (unsigned char)g1;
    		*(frame_buffer + ((IMAGEHEIGHT-1-i)*(IMAGEWIDTH/2)+j)*6 + 2) = (unsigned char)r1;
    		*(frame_buffer + ((IMAGEHEIGHT-1-i)*(IMAGEWIDTH/2)+j)*6 + 3) = (unsigned char)b2;
    		*(frame_buffer + ((IMAGEHEIGHT-1-i)*(IMAGEWIDTH/2)+j)*6 + 4) = (unsigned char)g2;
    		*(frame_buffer + ((IMAGEHEIGHT-1-i)*(IMAGEWIDTH/2)+j)*6 + 5) = (unsigned char)r2;
    	}
    }
    printf("change to RGB OK \n");
	return 0;
}


int close_v4l2(void)
{
     if(fd != -1) 
     {
         close(fd);
         return (TRUE);
     }
     return (FALSE);
}


int rgb2jpeg(const char * filename, unsigned char* rgbData, int image_width, int image_height,int quality)  
{  
    struct jpeg_compress_struct jpeg;  //identify a compress object
    struct jpeg_error_mgr jerr;  //error information
 
    jpeg.err = jpeg_std_error(&jerr);  
    jpeg_create_compress(&jpeg);  //init compress object
 
    FILE* pFile = fopen(filename, "wb");  
    if( !pFile )  
		return 0;

    jpeg_stdio_dest(&jpeg, pFile);  
 
    //compress param set,i just did a simple param set
    jpeg.client_data      = (void*)&pFile;
    jpeg.image_width      = image_width;  
    jpeg.image_height     = image_height;  
    jpeg.input_components = 3;  
    jpeg.in_color_space   = JCS_RGB;   
    jpeg_set_defaults(&jpeg);   
    //// 指定亮度及色度质量  
    jpeg.q_scale_factor[0] = jpeg_quality_scaling(100);  
    jpeg.q_scale_factor[1] = jpeg_quality_scaling(100);  
    //// 图像采样率，默认为2 * 2  
    jpeg.comp_info[0].v_samp_factor = 2;  
    jpeg.comp_info[0].h_samp_factor = 2;  
    //// set jpeg compress quality  
    jpeg_set_quality(&jpeg, quality, TRUE);  //100 is the highest
 
    //start compress
    jpeg_start_compress(&jpeg, TRUE);  
 
    JSAMPROW row_pointer[1];  
 
    //from up to down ,set every pixel
    for(unsigned int i = 0; i < jpeg.image_height; i++ )  
    {
        row_pointer[0] = rgbData + i * jpeg.image_width * 3;  
        jpeg_write_scanlines(&jpeg, row_pointer, 1);  
    }  

    //stop compress
    jpeg_finish_compress(&jpeg);  
    fclose( pFile );  
    pFile = NULL;  
    jpeg_destroy_compress(&jpeg);  
    return 0;  
}





int main(void)
{

    FILE* fp1;
	FILE* fp2_v0;
	FILE* fp2_v1;
	FILE* fp2_v2;
	FILE* fp2_v3;

    int grab;
    BITMAPFILEHEADER   bf;
    BITMAPINFOHEADER   bi;


	//due to the camera is not ready, some of the begin frames are not good,
	//maybe we should skip the frame
    fp1 = fopen(BMP, "wb");
    if(!fp1)
	{
		printf("open "BMP"error\n");
		return(FALSE);
	}
	
	fp2_v0 = fopen("./v0.yuv", "wb");
    if(!fp2_v0)
	{
		printf("creat new file error\r\n");
		return(FALSE);
	}


	fp2_v1 = fopen("./v1.yuv", "wb");
    if(!fp2_v1)
	{
		printf("creat new file error\r\n");
		return(FALSE);
	}

	fp2_v2 = fopen("./v2.yuv", "wb");
    if(!fp2_v2)
	{
		printf("creat new file error\r\n");
		return(FALSE);
	}


	fp2_v3 = fopen("./v3.yuv", "wb");
    if(!fp2_v3)
	{
		printf("creat new file error\r\n");
		return(FALSE);
	}



	if(init_v4l2() == FALSE) 
	{
     	return(FALSE);
	}
	
	//Set BITMAP INFO HEADER
	bi.biSize          = 40;
	bi.biWidth         = IMAGEWIDTH;
	bi.biHeight        = IMAGEHEIGHT;
	bi.biPlanes        = 1;
	bi.biBitCount      = 24;
	bi.biCompression   = 0;
	bi.biSizeImage     = IMAGEWIDTH * IMAGEHEIGHT * 3;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed       = 0;
	bi.biClrImportant  = 0;
 

    //Set BITMAP FILE HEADER
    bf.bfType      = 0x4d42;
    bf.bfSize      = 54 + bi.biSizeImage;     
    bf.bfReserved1 = 0;
	bf.bfReserved2 = 0;
    bf.bfOffBits   = 54;


    grab = v4l2_grab();
    printf("GRAB is %d\n",grab);
    printf("grab ok\n");

    fwrite(buffers[0].start, IMAGEHEIGHT*IMAGEWIDTH*2,1, fp2_v0);
	fwrite(buffers[1].start, IMAGEHEIGHT*IMAGEWIDTH*2,1, fp2_v1);
	fwrite(buffers[2].start, IMAGEHEIGHT*IMAGEWIDTH*2,1, fp2_v2);
	fwrite(buffers[3].start, IMAGEHEIGHT*IMAGEWIDTH*2,1, fp2_v3);

    
    //yuyv_2_rgb888();
	convert_yuv_to_rgb_buffer(buffers[0].start, frame_buffer, IMAGEWIDTH, IMAGEHEIGHT);

	rgb2jpeg("./myself.jpeg", frame_buffer, IMAGEWIDTH, IMAGEHEIGHT, 100);

    fwrite(&bf, 14, 1, fp1);
    fwrite(&bi, 40, 1, fp1);    
    fwrite(frame_buffer, bi.biSizeImage, 1, fp1);
    printf("save "BMP"OK\n");
    
    
    fclose(fp1);
    fclose(fp2_v0);
	fclose(fp2_v1);
	fclose(fp2_v2);
	fclose(fp2_v3);
    close_v4l2();
    
    return(TRUE);
}
