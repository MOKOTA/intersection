#include<opencv2/core.hpp>
#include<opencv2/core/utility.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/video/background_segm.hpp>
#include<opencv2/videoio.hpp>
#include<opencv2/highgui.hpp>
#include<string>
#ifndef VIDEO_H
#define VIDEO_H
#define HEIGHT 200
#define WIDTH  200
//using namespace cv;
class video
{
	std::string Filename;
	cv::VideoCapture Cap;
	bool updateforeimg;
	cv::Mat foreimg;
	cv::Mat backimg;
	bool success;
private:
	/*返回当前时间轴*/
	void returntime();
	/*
	* video()默认构造函数
	* video(string)指定视频文件构造函数*
	* pause暂停
	* fastforward和slowdown友元函数用来改变视频播放及处理速度*/
public:
	video();
	video(std::string);
	void pause();
	friend void fastforward(float speed);
	friend void slowdown(float speed);
	friend void threashold_binary(cv::Mat &);
	friend void adjust_smooth(cv::Mat &);
	void pre_soulution();
};
#endif
