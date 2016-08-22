#include"video.h"
#include<opencv2/video/background_segm.hpp>
using namespace cv;

video::video()
{
	Filename = '\0';
	Cap = NULL;
	updateforeimg = 1;
	success = false;
}

video::video(std::string file)
{
	Filename = file;
	Cap.open(Filename.c_str());
	updateforeimg = 1;
	
}
void video::pause()
{//TUDO
}
void threathold_binary(Mat &src)
{
	if (src.type() != CV_8UC1)
		src.convertTo(src, CV_8UC1);
	adaptiveThreshold(src, src, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 3, 3);//param 3*3 should be modified
}






