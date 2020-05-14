# v4l2_yuv_h264
V4l2 capture the YUV image and then use x264 encoding into h264
step:
1.init camera device
2.get one frame
3.convert yuyv to I420p (yu12)
4.encode with h264
5.fwrite into file
6.close
