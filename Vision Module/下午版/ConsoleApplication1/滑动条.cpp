#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>  
#include <stdio.h>  


using namespace std;
using namespace cv;

int ilowh = 90;
int ihighh = 179;
int ilows = 80;
int ihighs = 255;
int ilowv = 0;
int ihighv = 255;
int value = 112;
void onThreshold(int, void*);
Mat midImage,dsImage;
Mat asrc;
int cmin = 150;
int cmax = 250;

int main(int argc, char** argv)
{
	asrc = imread("3.jpg");
	imshow("原图", asrc);
	//【3】转为灰度图并进行图像平滑
	cvtColor(asrc, midImage, CV_BGR2GRAY);//转化边缘检测后的图为灰度图
	GaussianBlur(midImage, midImage, Size(5, 5), 2, 2);


//定义一些参数
	namedWindow("阈值二值化");
	namedWindow("drawing");
	/*
	// 使用Threshold检测边缘，对灰度图像进行阈值操作得到二值图像，进而用findContours在二值图中找出轮廓。
	//createTrackbar("阈值", "阈值二值化", &value, 255, onThreshold);
	createTrackbar("cmin", "阈值二值化", &cmin, 255, onThreshold);
	createTrackbar("cmax", "阈值二值化", &cmax, 255, onThreshold);
	*/
	createTrackbar("ilowh", "阈值二值化", &ilowh, 179, onThreshold);
	createTrackbar("ihighh", "阈值二值化", &ihighh, 179, onThreshold);
	createTrackbar("ilows", "阈值二值化", &ilows, 255, onThreshold);
	createTrackbar("ihighs", "阈值二值化", &ihighs, 255, onThreshold);
	createTrackbar("ilowv", "阈值二值化", &ilowv, 255, onThreshold);
	createTrackbar("ihighv", "阈值二值化", &ihighv, 255, onThreshold);


	onThreshold(value, 0);

	waitKey(0);

	return 0;
}




void onThreshold(int, void*)
{
	/*vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Mat	threshold_output;
	threshold(midImage, threshold_output, value, 255, THRESH_BINARY);
	imshow("阈值化后得到的二值图像", threshold_output);

	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));



	//创建常量迭代器，搜索满足条件的（轮廓长度在XX到XX之间的轮廓）
	vector<vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if ((itc->size()<cmin) || (itc->size()>cmax))      //size是轮廓的长度
			itc = contours.erase(itc);
		else
			++itc;
	}


	double a;
	Mat imgROI;
	//获取圆形边界框
	vector<vector<Point> > contours_poly(contours.size());           //用于存放折线点集，近似后的轮廓点集?????, contours.size()表示符合之前要求的轮廓的个数
	//vector<Rect> boundRect( contours.size() );
	vector<Point2f>center(contours.size());                        //包围点集的最小圆形vector
	vector<float>radius(contours.size());                         //包围点集的最小圆形半径vector
	//一个循环，遍历所有部分，进行本程序最核心的操作
	for (unsigned int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);//用指定精度3逼近多边形曲线()轮廓 ，contours_poly[i]是输出的近似点集
		minEnclosingCircle(contours_poly[i], center[i], radius[i]);//对给定的 2D点集，即根据approxPolyDP函数得出的曲线，寻找最小面积的包围圆形 
	}
	
	// 绘制 圆形框
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
	//imshow("yyy", drawing);
	//int j = 0;
	for (int unsigned i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(255, 255, 160);//设置淡蓝色
		drawContours(drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());//绘制轮廓，注意因为绘制的仅仅是逼近的多边形曲线
		circle(drawing, center[i], (int)radius[i], color, 2, 8, 0);//绘制圆
		//imshow("zzz", drawing);

		a = 3.14*radius[i] * radius[i];
		cout << "圆心坐标" << center[i] << endl;
		cout << "球的半径" << radius[i] << endl;
		cout << "圆的面积" << a << endl;
*/

		
		//颜色识别,输出二值图，指定颜色为白色
		Mat imgHSV, imgThresholded;
		Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
		vector<Mat> hsvSplit;
		cvtColor(asrc, imgHSV, CV_BGR2HSV);
		split(imgHSV, hsvSplit);
		equalizeHist(hsvSplit[2], hsvSplit[2]);
		merge(hsvSplit, imgHSV);
		inRange(imgHSV, Scalar(ilowh, ilows, ilowv), Scalar(ihighh, ihighs, ihighv), imgThresholded);
		morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
		morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);
		imshow("颜色识别inRange二值图", imgThresholded);


	/*	if (((center[i].x - radius[i])>0) && ((center[i].y - radius[i]) > 0) &&
		((center[i].x - radius[i]) < 640) && ((center[i].y - radius[i]) < 480))
		{
		imgROI = imgThresholded(Rect((center[i].x - radius[i]), (center[i].y - radius[i]), 2 * radius[i], 2 * radius[i]));  //感兴趣区域为包含圆的正方形
		double b = 0;
		for (int k = 0; k < imgROI.rows; ++k)
		{
		for (int j = 0; j < imgROI.cols; ++j)
		{
		if (imgROI.at<uchar>(k, j) == 255)   b++;
		}
		}

		cout << a << " " << b << " " << b / a << endl;
		if ((b / a) > 0.2)                                    //?????
		{
		cout << i << "目标球" << endl;
		//point = center[i].x;
		}
		else  cout << i << "其他球" << endl;
		}
		}
		imshow("drawing", drawing);*/
}