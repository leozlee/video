#include <stdio.h>
#include <stdlib.h>
#include "video_capture.h"


int main(void) 
{
    unsigned char src_image[IM_WIDTH * IM_HEIGHT * 3];
	init_video_capture();
	char key = 0;

	for(; ;)
	{
		key = video_capture(src_image);
		if(key == 'q'){
			break;
		}
	}
    free_video_capture();
	return EXIT_SUCCESS;
}
