#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<stdint.h>
#include<time.h>
#include"video.h"
/*�ص�����*/
struct position
{
	float location_x;
	float location_y;
};
/*ת��ö��*/
enum turn{stright,left,right,uturn};


/*
-------------����----------------
����������������ID��ת�򣬳���������ʱ��
*/
class car
{
	cv::Mat image;
	int id;
	enum track{ stright, left, right, uturn };
	int lane;
	long time;
public:
	car();
	car(video,position,float)
	~car();
	friend turn trace();//�����㷨

};
