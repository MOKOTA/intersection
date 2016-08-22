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
	/*���ص�ǰʱ����*/
	void returntime();
	/*
	* video()Ĭ�Ϲ��캯��
	* video(string)ָ����Ƶ�ļ����캯��*
	* pause��ͣ
	* fastforward��slowdown��Ԫ���������ı���Ƶ���ż������ٶ�*/
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
