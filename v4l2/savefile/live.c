#include <stdio.h>
#include <stdlib.h>
#include "video_capture.h"
#include <strings.h>

int main(void) 
{
	FILE *fp = NULL;
	fp = fopen("./myvideo.yuv", "wa+");
	init_video_capture();
	char key = 0;

	for(; ;)
	{
		key = video_capture(fp);
		if(key == 'q'){
			break;
		}

		

	}
	fclose(fp);
    free_video_capture();
	return EXIT_SUCCESS;
}
