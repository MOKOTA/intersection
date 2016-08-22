#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<stdint.h>
#include<time.h>
#include"video.h"
/*地点坐标*/
struct position
{
	float location_x;
	float location_y;
};
/*转向枚举*/
enum turn{stright,left,right,uturn};


/*
-------------车辆----------------
数据特征包括基本ID，转向，车道，出现时间
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
	friend turn trace();//跟踪算法

};
