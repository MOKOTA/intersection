#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <stdio.h>
#include<deque>

using namespace std;
using namespace cv;

static void help()
{
	printf("\nDo background segmentation, especially demonstrating the use of cvUpdateBGStatModel().\n"
		"Learns the background at the start and then segments.\n"
		"Learning is togged by the space key. Will read from file or camera\n"
		"Usage: \n"
		"			./bgfg_segm [--camera]=<use camera, if this key is present>, [--file_name]=<path to movie file> \n\n");
}

const char* keys =
{
	"{c  camera   |         | use camera or not}"
	"{m  method   |mog2     | method (knn or mog2) }"
	"{s  smooth   |         | smooth the mask }"
	"{fn file_name|../data/tree.avi | movie file        }"
};
void m_binary(Mat &src)
{
//	printf("%d",CV_8UC3);
	//printf("%d", CV_8UC1);
	if (src.type() != CV_8UC1)
		src.convertTo(src, CV_8UC1);
	//printf("%d", src.type());
	adaptiveThreshold(src, src, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 5, 5);
}

vector<vector<Point>> feedback_position(Mat src)
{
				//CvMemStorage *pcmStorage = cvCreateMemStorage();
				//CvSeq *pcvSeq = NULL;
				std::vector<std::vector<cv::Point> > contours;
				findContours(src, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
				Mat contour_img(src.size(), CV_8UC3, cv::Scalar(0, 0, 0));
				//Mat contour_img(fgmask.size(), IPL_DEPTH_8U, 3);
				//int nLevels = 5;
				//cvRectangle(&contour_img, cvPoint(0, 0), cvPoint(contour_img.cols, contour_img.rows), CV_RGB(255, 255, 255), CV_FILLED);
				cv::Scalar colors[3];
				colors[0] = cv::Scalar(255, 0, 0);
				colors[1] = cv::Scalar(0, 255, 0);
				colors[2] = cv::Scalar(0, 0, 255);
				float area = 0;
				float max_area = src.rows*src.cols;
				for (size_t idx = 0; idx < contours.size(); idx++) {
					area = fabs(contourArea(contours[idx]));
					if (area>max_area / 4.0 || area < 100)
						continue;
					cv::drawContours(contour_img, contours, idx, colors[idx % 3]);
				}
				return contours;
				//drawContours(contour_img, contours, CV_RGB(255, 0, 0), CV_RGB(0, 255, 0), nLevels, 2);
				//cvReleaseMemStorage(&pcmStorage);
	
}
//this is a sample for foreground detection functions
int main(int argc, const char** argv)
{
	help();

	CommandLineParser parser(argc, argv, keys);
	bool useCamera = parser.has("camera");
	bool smoothMask = parser.has("smooth");
//	string file = parser.get<string>("file_name");
	//string method = parser.get<string>("method");
	string method = "knn";
	VideoCapture cap;
	bool update_bg_model = true;
	/*
	if (useCamera)
		cap.open(0);
	else
		cap.open(file.c_str());

	parser.printMessage();
	*/
	string file = "D:\\c++\\intersection\\conflict.flv";
	//testimg.release();
	cap.open(file.c_str());
	if (!cap.isOpened())
	{
		printf("can not open camera or video file\n");
		return -1;
	}

	namedWindow("image", WINDOW_NORMAL);
	//namedWindow("binary", WINDOW_NORMAL);
	
	namedWindow("foreground mask", WINDOW_NORMAL);
	namedWindow("foreground image", WINDOW_NORMAL);
	namedWindow("mean background image", WINDOW_NORMAL);

	Ptr<BackgroundSubtractor> bg_model = method == "knn" ?
		createBackgroundSubtractorKNN(1000,400.0,1).dynamicCast<BackgroundSubtractor>() :
		createBackgroundSubtractorMOG2().dynamicCast<BackgroundSubtractor>();

	Mat img0, img, fgmask, fgimg;
	//int framecount = 1;
	double t = (double)getTickCount();
	int current_frame = 0;
	for (;;)
	{
		cap >> img0;

		if (img0.empty())
			break;

		resize(img0, img, Size(640, 640 * img0.rows / img0.cols), INTER_LINEAR);

		if (fgimg.empty())
			fgimg.create(img.size(), img.type());

		//update the model
		current_frame = cap.get(CV_CAP_PROP_POS_FRAMES);
		bg_model->apply(img, fgmask, update_bg_model ? -1 : 0);

		if (1)
		{
			
			medianBlur(fgmask, fgmask, 3);//(fgmask, fgmask, Size(11, 11), 3.5, 3.5);
			//threshold(fgmask, fgmask, 10, 255, THRESH_BINARY);
			adaptiveThreshold(fgmask, fgmask, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 3, 3);
			if (current_frame > 150)
			{
				vector<vector<Point>> contours;
				contours = feedback_position(fgmask);
				vector<vector<cv::Point>> null;
				null.swap(contours);
			}
			//Mat temp(fgmask.size(), IPL_DEPTH_8U, 1);
			//temp = fgmask.clone();
			//resize(temp, img, Size(640, 640 * img0.rows / img0.cols), INTER_LINEAR);
			//	if (fgimg.empty())
			//	fgimg.create(img.size(), img.type());
			//Mat img_gray;
			//cvtColor(fgmask, fgmask, CV_RGB2GRAY);
			//m_binary(fgmask);
			//imshow("image", img);
			//char k = (char)waitKey(10);
			//if (k == 27) break;
			//t = ((double)getTickCount() - t) / getTickFrequency();
			//temp.release();
		}

		fgimg = Scalar::all(0);
		img.copyTo(fgimg, fgmask);

		Mat bgimg;
		bg_model->getBackgroundImage(bgimg);

		imshow("image", img);
		imshow("foreground mask", fgmask);
		imshow("foreground image", fgimg);
		if (!bgimg.empty())
			imshow("mean background image", bgimg);
		//framecount++;
		char k = (char)waitKey(10);
		if (k == 27) break;
		t = ((double)getTickCount() - t) / getTickFrequency();
		/*
		if (1)
		{
			update_bg_model = !update_bg_model;
			if (update_bg_model)
				printf("Background update is on\n");
			else
				printf("Background update is off\n");
		}*/
	}
	/*
	Mat img0, img, fgmask, fgimg;
	//int framecount = 1;
	double t = (double)getTickCount();
	for (;;)
	{
		cap >> img0;

		if (img0.empty())
			break;
		Mat temp(img0.size(),IPL_DEPTH_8U,1);
		temp = img0.clone();
		//temp = img0.clone();
		//Mat temp;
		resize(temp, img, Size(640, 640 * img0.rows / img0.cols), INTER_LINEAR);
	//	if (fgimg.empty())
		//	fgimg.create(img.size(), img.type());
		//Mat img_gray;
		cvtColor(img, img, CV_RGB2GRAY);
		m_binary(img);
		imshow("image", img);
		char k = (char)waitKey(10);
		if (k == 27) break;
		t = ((double)getTickCount() - t) / getTickFrequency();
		temp.release();
	}*/
	cap.release();

	return 0;
}
