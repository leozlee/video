#ifndef _H264ENCODER_H
#define _H264ENCODER_H

#include <stdint.h>
#include <stdio.h>
#include <x264.h>



#define PIC_HEIGTH 600
#define PIC_WIDTH  800	


#define true  1
#define false 0 

typedef unsigned char uint8_t;

typedef struct {
	x264_param_t *param;
	x264_t *handle;
	x264_picture_t *picture; 
	x264_nal_t *nal;
} Encoder;


void compress_begin(Encoder *en, int width, int height);

int compress_frame(Encoder *en, int type, uint8_t *in, uint8_t *out);

void compress_end(Encoder *en);

#endif

