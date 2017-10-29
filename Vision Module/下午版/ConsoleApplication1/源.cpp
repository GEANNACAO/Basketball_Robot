#include <iostream>
#include<core/core.hpp>  
#include<highgui/highgui.hpp>  
#include "opencv2/imgproc/imgproc.hpp"
#include <windows.h>
#include <winnt.h>
#include<XnCppWrapper.h>
#pragma comment( lib, "OpenNI.lib" )
#ifdef _DEBUG
#pragma comment(lib, "opencv_core2413d.lib" )
#pragma comment(lib, "opencv_highgui2413d.lib" )
#pragma comment(lib, "opencv_imgproc2413d.lib" )
#else
#pragma comment(lib, "opencv_core2413.lib" )
#pragma comment(lib, "opencv_highgui2413.lib" )
#pragma comment(lib, "opencv_imgproc2413.lib" )
#endif
using namespace cv;
using namespace std;
HANDLE hCom;

int ZQD_ComInit()
{
	hCom = CreateFile(TEXT("COM7"),//COM1口
		GENERIC_READ | GENERIC_WRITE, //允许读和写
		0, //独占方式
		NULL,
		OPEN_EXISTING, //打开而不是创建
		0, //同步方式
					NULL);
	if (hCom == (HANDLE)-1)
	{
		MessageBox(NULL, TEXT("打开COM失败"), TEXT("Error"), MB_OK);
		CloseHandle(hCom);
		return -1;
	}

	COMMTIMEOUTS TimeOuts;
	//设定读超时
	TimeOuts.ReadIntervalTimeout = 10;
	TimeOuts.ReadTotalTimeoutMultiplier = 20;
	TimeOuts.ReadTotalTimeoutConstant = 20;
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 500;
	TimeOuts.WriteTotalTimeoutConstant = 2000;
	// 写入串口超时参数
	if (!SetCommTimeouts(hCom, &TimeOuts))
	{
		MessageBox(NULL, TEXT("写入超时参数错误"), TEXT("Error"), MB_OK);
		CloseHandle(hCom);
		return -1;
	}

	// 设置输入输出缓冲区参数，返回非0表示成功 
	if (!SetupComm(hCom, 1024, 1024))
	{
		MessageBox(NULL, TEXT("设置串口读写缓冲区失败"), TEXT("Error"), MB_OK);
		CloseHandle(hCom);
		return -1;
	}

	DCB dcb;
	// 获取当前串口状态信息(只需要修改部分串口信息),调用失败返回0
	if (!GetCommState(hCom, &dcb))
	{
		MessageBox(NULL, TEXT("获取串口属性失败"), TEXT("Error"), MB_OK);
		CloseHandle(hCom);
		return -1;
	}
	dcb.BaudRate = 9600; //波特率为9600
	dcb.ByteSize = 8; //每个字节有8位
	dcb.Parity = NOPARITY; //无奇偶校验位
	dcb.StopBits = ONESTOPBIT; //一个停止位
	if (!SetCommState(hCom, &dcb))
	{
		MessageBox(NULL, TEXT("设置串口参数出错"), TEXT("Error"), MB_OK);
		CloseHandle(hCom);
		return -1;
	}

	return 0;
}
// 全局变量定义及赋值
int point = 0;
int juli = 0;
Mat g_c8BitDepth;
vector<Mat> hsvSplit;
Mat hsv_image;
Mat img;


void NIfunction(int, void*)
{
	/*int x=3500.0/7000*255;
	for (int j = 50; j < 400; j++)
	{
		if ((g_c8BitDepth.at<uchar>(50, j) < (3500.0 / 7000 * 255)) && (g_c8BitDepth.at<uchar>(50, j) > (1000.0 / 7000 * 255)))
		{
			if (x>g_c8BitDepth.at<uchar>(50,j))
			x = g_c8BitDepth.at<uchar>(50, j);
		}
		
	}*/
	for (int i = 0; i < g_c8BitDepth.rows; i++)
	for (int j = 0; j< g_c8BitDepth.cols; j++)
	if (g_c8BitDepth.at<uchar>(i, j) >=1.0*1800/7000*255 )
	{
			g_c8BitDepth.at<uchar>(i, j) = 0;
		}
#ifdef _debug
	imshow("depth", g_c8BitDepth);
#endif
}

void getSizeContours(vector<vector<Point>>&contours)
{
	int cmin = 100;// 最小轮廓长度  
	int cmax = 10000;// 最大轮廓长度  
	vector<vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if ((itc->size()) < cmin || (itc->size())>cmax)
		{
			itc = contours.erase(itc);
		}
		else ++itc;
	}
}

void getAreaContours(vector<vector<Point>>&contours)
{
	int amin =700;// 最小轮廓面积  ,700
	int amax = 20000;// 最大轮廓面积,80cm-350cm,18000
	for (int i = 0; i < (int)contours.size(); i++)
	{
		double g_dConArea = fabs(contourArea(contours[i], true));

	}
	//筛选剔除掉面积不合适的轮廓
	vector <vector<Point>>::iterator itc = contours.begin();
	for (; itc != contours.end();)
	{
		double g_dConArea = contourArea(*itc);
		if ((g_dConArea < amin) || (g_dConArea >amax))
		{
			itc = contours.erase(itc);
		}
		else
		{
			++itc; 
		}

	}
	//筛选后的轮廓的面积
	for (int i = 0; i < (int)contours.size(); i++)
	{
		double g_dConArea = fabs(contourArea(contours[i], true));
	}
}
//经轮廓周长和面积判断后筛选轮廓




double bluede(Mat asrc){
	Mat image = asrc;
	cv::Mat hsv_image;        //转HSV
	hsv_image.create(image.size(), image.type());
	cv::cvtColor(image, hsv_image, CV_BGR2HSV);
	vector<cv::Mat> channels;
	cv::split(hsv_image, channels);
	int num_row = image.rows;
	int num_col = image.cols;
	int count = 0;
	int count_blue = 0;
	for (int i = 0; i < num_row; i += 2)
	{
		const cv::Vec3b* curr_r_image = image.ptr<const cv::Vec3b>(i);
		const uchar* curr_r_hue = channels[0].ptr<const uchar>(i);
		const uchar* curr_r_satur = channels[1].ptr<const uchar>(i);
		const uchar* curr_r_value = channels[2].ptr<const uchar>(i);
		const uchar*inData = image.ptr<uchar>(i);
		//	uchar*outData = mask.ptr<uchar>(i);
		for (int j = 0; j < num_col; j += 2){
			if ((curr_r_hue[j] <= 130 && curr_r_hue[j] >= 90) && (curr_r_satur[j]>24 && curr_r_satur[j]<255))
			{
				count_blue++; count++;
			}
			else  count++;
		}
	}
	double blue;
	blue = 1.0*count_blue / count;
#ifdef _debug
	
	cout << "blue   " << blue << endl;
#endif    

	return blue;


}
double yellowde(Mat asrc){
	Mat image = asrc;
	cv::Mat hsv_image;        //转HSV
	hsv_image.create(image.size(), image.type());
	cv::cvtColor(image, hsv_image, CV_BGR2HSV);
	vector<cv::Mat> channels;
	cv::split(hsv_image, channels);
	int num_row = image.rows;
	int num_col = image.cols;
	int count = 0;
	int count_yellow = 0;
	for (int i = 0; i < num_row; i += 2)
	{
		const cv::Vec3b* curr_r_image = image.ptr<const cv::Vec3b>(i);
		const uchar* curr_r_hue = channels[0].ptr<const uchar>(i);
		const uchar* curr_r_satur = channels[1].ptr<const uchar>(i);
		const uchar* curr_r_value = channels[2].ptr<const uchar>(i);
		const uchar*inData = image.ptr<uchar>(i);
		//	uchar*outData = mask.ptr<uchar>(i);
		for (int j = 0; j < num_col; j += 2){
			if ((curr_r_hue[j] <= 34 && curr_r_hue[j] >= 16) && (curr_r_satur[j]>80 && curr_r_satur[j] < 255))
			{
				count_yellow++; count++;
			}
			else  count++;
		}
	}
	double yellow;
	yellow = 1.0*count_yellow / count;

#ifdef _debug
	cout << "count" << count << endl;
	cout << "yellow   " << yellow << endl;
#endif    

	return yellow;
}
double whitede(Mat asrc){
	Mat image = asrc;
	cv::Mat hsv_image;        //转HSV
	hsv_image.create(image.size(), image.type());
	cv::cvtColor(image, hsv_image, CV_BGR2HSV);
	vector<cv::Mat> channels;
	cv::split(hsv_image, channels);
	int num_row = image.rows;
	int num_col = image.cols;
	int count = 0;
	int count_white = 0;

	//Mat mask(cv::Size(num_col, num_row), CV_8U);
	for (int i = 0; i < num_row; i += 2)
	{
		const cv::Vec3b* curr_r_image = image.ptr<const cv::Vec3b>(i);
		const uchar* curr_r_hue = channels[0].ptr<const uchar>(i);
		const uchar* curr_r_satur = channels[1].ptr<const uchar>(i);
		const uchar* curr_r_value = channels[2].ptr<const uchar>(i);
		const uchar*inData = image.ptr<uchar>(i);
		//	uchar*outData = mask.ptr<uchar>(i);
		for (int j = 0; j < num_col; j += 2){
			if ((curr_r_hue[j] <= 180 && curr_r_hue[j] >= 0) && (curr_r_satur[j]>0 && curr_r_satur[j]<30) && (curr_r_value[j]>221 && curr_r_value[j]<255))//找颜色
			{
				count_white++; count++;
			}
		
			else  count++;
		}
	}

	double white;
	white = 1.0*count_white / count;
#ifdef _debug
	cout << "white   " << white << endl;
#endif    

	return (white);

}
double grayde(Mat asrc){
	Mat image = asrc;
	cv::Mat hsv_image;        //转HSV
	hsv_image.create(image.size(), image.type());
	cv::cvtColor(image, hsv_image, CV_BGR2HSV);
	vector<cv::Mat> channels;
	cv::split(hsv_image, channels);
	int num_row = image.rows;
	int num_col = image.cols;
	int count = 0;
	int count_gray = 0;
	
	//Mat mask(cv::Size(num_col, num_row), CV_8U);
	for (int i = 0; i < num_row; i += 2)
	{
		const cv::Vec3b* curr_r_image = image.ptr<const cv::Vec3b>(i);
		const uchar* curr_r_hue = channels[0].ptr<const uchar>(i);
		const uchar* curr_r_satur = channels[1].ptr<const uchar>(i);
		const uchar* curr_r_value = channels[2].ptr<const uchar>(i);
		const uchar*inData = image.ptr<uchar>(i);
		//	uchar*outData = mask.ptr<uchar>(i);
		for (int j = 0; j < num_col; j += 2){
			if ((curr_r_hue[j] <= 180 && curr_r_hue[j] >= 0) && (curr_r_satur[j]>0 && curr_r_satur[j]<43) && (curr_r_value[j]>46 && curr_r_value[j]<220))//找颜色
			{
				count_gray++; count++;
			}
			
			else  count++;
		}
	}

	double gray;
	gray = 1.0*count_gray / count;

#ifdef _debug
	cout << "gray   " << gray << endl;
#endif    

	return (gray);

}
double redde(Mat asrc){
	Mat image = asrc;
	cv::Mat hsv_image;        //转HSV
	hsv_image.create(image.size(), image.type());
	cv::cvtColor(image, hsv_image, CV_BGR2HSV);
	vector<cv::Mat> channels;
	cv::split(hsv_image, channels);
	int num_row = image.rows;
	int num_col = image.cols;
	int count = 0;
	int count_red1 = 0;
	int count_red2 = 0;

	//Mat mask(cv::Size(num_col, num_row), CV_8U);
	for (int i = 0; i < num_row; i += 2)
	{
		const cv::Vec3b* curr_r_image = image.ptr<const cv::Vec3b>(i);
		const uchar* curr_r_hue = channels[0].ptr<const uchar>(i);
		const uchar* curr_r_satur = channels[1].ptr<const uchar>(i);
		const uchar* curr_r_value = channels[2].ptr<const uchar>(i);
		const uchar*inData = image.ptr<uchar>(i);
		//	uchar*outData = mask.ptr<uchar>(i);
		for (int j = 0; j < num_col; j += 2){
			if ((curr_r_hue[j] <= 10 && curr_r_hue[j] >= 0) && (curr_r_satur[j]>60 && curr_r_satur[j]<255) && (curr_r_value[j]>46 && curr_r_value[j]<200))//找颜色
			{
				count_red1++; count++;
			}
			else if ((curr_r_hue[j] <= 180 && curr_r_hue[j] >= 160) && (curr_r_satur[j]>60 && curr_r_satur[j]<255) && (curr_r_value[j]>46 && curr_r_value[j]<200))
			{
				count_red2++; count++;
			}
			else  count++;
		}
	}

	double red1, red2;
	red1 = 1.0*count_red1 / count;
	red2 = 1.0*count_red2 / count;
#ifdef _debug
	cout << "red   " << red1 + red2 << endl;
#endif    

	return (red1 + red2);

}
Mat bianyuanjiance(Mat asrc)
{
	Mat dst,edges;
	int radio = 3;
	int size = 3;
	blur(asrc, edges, Size(3, 3));
	Canny(edges, edges, 50, 50 * radio, 3);
	dst = Scalar::all(0);
	asrc.copyTo(dst, edges);
//	imshow("edges", dst);
	return dst;
}

void picprocess(Mat asrc, Mat depth, int data)
{
	Mat  srcImage = depth;
	int a = data;
	Mat midImage, dstImage;//临时变量和目标图的定义
	//【3】转为灰度图并进行图像平滑
	//cvtColor(srcImage, midImage, CV_BGR2GRAY);//转化边缘检测后的图为灰度图
	//GaussianBlur(midImage, midImage, Size(5, 5), 2, 2);
	//	sharpenImage(midImage, midImage);
	//定义一些参数
	Mat threshold_output;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	// 使用Threshold检测边缘
	threshold(srcImage, threshold_output, 20,255, THRESH_BINARY);//20
	//threshold_output = bianyuanjiance(srcImage);
#ifdef _debug
	imshow("二值化", threshold_output);
#endif
	// 找出轮廓
	findContours(threshold_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

#ifdef _debug
	cout << "【面积筛选前总共轮廓个数为：" << (int)contours.size() << endl;
	Mat result1(threshold_output.size(), CV_8U, Scalar(0));
	drawContours(result1, contours, -1, Scalar(255), 1);
	imshow("筛选轮廓前", result1);
#endif
	getAreaContours(contours);

#ifdef _debug
	cout << "【面积筛选后总共轮廓个数为：" << (int)contours.size() << endl;
	Mat result2(threshold_output.size(), CV_8U, Scalar(0));
	drawContours(result2, contours, -1, Scalar(255), 1);
	imshow("筛选轮廓后", result2);
#endif
	Mat imgROI;
	// 多边形逼近轮廓 + 获取矩形和圆形边界框
	vector<vector<Point> > contours_poly(contours.size());
	//vector<Rect> boundRect( contours.size() );
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());
	//一个循环，遍历所有部分，进行本程序最核心的操作
	int flag = -1; double index0 = 0, index1 = 0,index2 = 0;
	for (unsigned int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);//用指定精度逼近多边形曲线 
		minEnclosingCircle(contours_poly[i], center[i], radius[i]);//对给定的 2D点集，寻找最小面积的包围圆形 
	}
	// 绘制多边形轮廓 + 圆形框
#ifdef _debug
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
#endif
	for (int unsigned i = 0; i < contours.size(); i++)
	{
#ifdef _debug
		Scalar color = Scalar(0, 255, 0);//设置绿色
		drawContours(drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());//绘制轮廓
		circle(drawing, center[i], (int)radius[i], color, 2, 8, 0);//绘制圆
		circle(srcImage, center[i], 3, Scalar(0, 255, 255), -1, 8, 0);
		//a = 3.14*radius[i] * radius[i];
#endif
		if (((center[i].x - radius[i])>0) && ((center[i].y - radius[i]) > 0) &&
			((center[i].x + radius[i]) <srcImage.cols) && ((center[i].y + radius[i]) < srcImage.rows) && (radius[i]>10))
		{
			imgROI = asrc(Rect((center[i].x - radius[i]), (center[i].y - radius[i]), 2 * radius[i], 2 * radius[i]));
	
			double red, blue, yellow;
			double white, gray;
			double per;
			if (a == 1)
			{
				yellow = yellowde(imgROI);
				blue = bluede(imgROI);
				red = redde(imgROI);
				per = 1.0*yellow / red;
				cout << per << endl;
				if (((red >= index0) && (red>0.50) && (blue<0.05) && (yellow<0.025)) || ((blue >= index1) && (blue>0.115) && (red<0.35) && (per>0.26) && (per<0.6))) {
					index0 = red;
					index1 = blue; 
					index2 = yellow;
					flag = i;
				}

			}
			else if (a == 3){
				yellow = yellowde(imgROI);
				blue = bluede(imgROI);
				red = redde(imgROI);
				per = 1.0*yellow / red;
				cout<<per << endl;
				if (((blue >= index1) && (blue<0.21) && (blue>0.02) && (red >= index0) && (red<0.42) && (per<0.25)) || ((yellow >= index2 && (yellow>0.04) && (red >= index0) && (red>0.1) && (red<0.55) && (blue<0.03)))){
					index0 = red;
					index1 = blue ;
					index2 = yellow;
					flag = i;
				}
			}
			else if (a == 5){
				gray = grayde(imgROI);
				white =whitede(imgROI);
				yellow = yellowde(imgROI);
				blue = bluede(imgROI);
				red = redde(imgROI);
				
			}

		}
	}
	if (flag>-1)
	{
#ifdef _debug
		cout << flag << "球" << center[flag].x << endl;
		cout << "球r  " << radius[flag] << endl;
		printf("%.2d\n", g_c8BitDepth.at<uchar>(center[flag].y, center[flag].x));
#endif
		cout << a << "球找到" << endl;
		cout << flag << "球" << center[flag].x << endl;
		point = center[flag].x;
		juli = 1.0*g_c8BitDepth.at<uchar>(center[flag].y, center[flag].x) / 255 * 7000;
		cout << juli << endl;
	}
	if (flag == -1)
		cout << a << "球未找到" << endl;
#ifdef _debug
	imshow("画圆", drawing);
	imshow("srcimage", srcImage);
#endif


}

int main()
{
	char com_data;
	ZQD_ComInit();
	//串口接收
	DWORD rCount;//接收的字节数
	char inputData[1];

	// 1a. initial OpenNI
	xn::Context xContext;
	xContext.Init();

	// 1b. create depth generator
	xn::DepthGenerator xDepth;
	xDepth.Create(xContext);

	// 1c. create image generator
	xn::ImageGenerator xImage;
	xImage.Create(xContext);


	XnMapOutputMode mapMode;
	mapMode.nXRes = 640;
	mapMode.nYRes = 480;
	//Set out mode  EHEIGTH
	mapMode.nFPS = 30;
	xImage.SetMapOutputMode(mapMode);
	xDepth.SetMapOutputMode(mapMode);


	// 1d. set alternative view point
	xDepth.GetAlternativeViewPointCap().SetViewPoint(xImage);


	// 3. start OpenNI
	xContext.StartGeneratingAll();
	while (1)
	{
		if (ReadFile(hCom, inputData, 1, &rCount, NULL));
		//double t = (double)cvGetTickCount();
		if (inputData[0] == '1' || inputData[0] == '3' || inputData[0] == '5')
			com_data = inputData[0];
		/*if (ReadFile(hCom, inputData, 1, &rCount, NULL) == 0)
		{
		MessageBoxA(NULL, "接收串口数据失败", MB_OK, 0);
		exit(1);
		}*/
	//	cout << inputData[0] << endl;

		// 4. update data
		xContext.WaitAndUpdateAll();


		// 5. get image data 只有颜色信息

		xn::ImageMetaData xColorData;
		xImage.GetMetaData(xColorData);

		// 5a. convert to OpenCV form
		cv::Mat r_cColorImg(xColorData.FullYRes(), xColorData.FullXRes(), CV_8UC3, (void*)xColorData.Data());

		// 5b. convert from RGB to BGR
		//cv::Mat cBGRImg;
		cvtColor(r_cColorImg, img, CV_RGB2BGR);

		// 6. get depth data 只有距离信息
		xn::DepthMetaData xDepthData;
		xDepth.GetMetaData(xDepthData);

		// 6a. convert to OpenCV form
		cv::Mat r_cDepthImg(xDepthData.FullYRes(), xDepthData.FullXRes(), CV_16UC1, (void*)xDepthData.Data());

		// 16b. convert to 8 bit
		r_cDepthImg.convertTo(g_c8BitDepth, CV_8U, 255.0 / 7000);
#ifdef _debug
		cv::imshow("Depth Image", g_c8BitDepth);
		

		// 6c. convert to 8bit, and do edge detection
		//cv::Mat CDepthEdge;
		//cv::Canny(c8BitDepth, cDepthImg, 5, 100);

		cv::imshow("original", img);//处理前

#endif
		NIfunction(0, 0);

		switch (com_data)
		{
		case '1':
			picprocess(img, g_c8BitDepth, 1);
			break;
		
		case '3':
			picprocess(img, g_c8BitDepth, 3);
			break;
		case '5':
			picprocess(img, g_c8BitDepth, 5);
			break;
		default:
			break;
		}

		//串口发送
		DWORD wCount;//发送的字节数
		char outputData[1024];
		if (point / 100)
		{
			outputData[0] = '0' + point / 100;
		}
		else
			outputData[0] = ' ';
		point = point % 100;
		if (point / 10)
			outputData[1] = '0' + point / 10;
		else
			outputData[1] = ' ';
		point = point % 10;
		if (point)
			outputData[2] = '0' + point;
		else
			outputData[2] = ' ';
		if (juli / 1000)
			outputData[3] = '0' + juli / 1000;
		else
			outputData[3] = ' ';
		juli = juli % 1000;
		if (juli / 100)
			outputData[4] = '0' + juli / 100;
		else
			outputData[4] = ' ';
		juli = juli % 100;
		if (juli/10)
			outputData[5] = '0' + juli/10;
		else
			outputData[5] = ' ';
		juli = juli % 10;
		if (juli)
			outputData[6] = '0' + juli;
		else
			outputData[6] = ' ';
		outputData[7] = 'z';
	    outputData[8] = outputData[0] + outputData[1] + outputData[2] + outputData[3] + outputData[4] + outputData[5]+outputData[6];
		if (outputData[8] == 'z')
			outputData[8] = outputData[8] + '1';

		if (WriteFile(hCom, outputData, 9, &wCount, NULL) == 0)
		{
			MessageBoxA(NULL, "写入串口数据失败", MB_OK, 0);
			CloseHandle(hCom);
			exit(1);
		}
		waitKey(1000);
		//t = ((double)cvGetTickCount() - t) / (cvGetTickFrequency() * 1000);
		//cout << "处理时间: " << t << "ms" << endl;
	}
	cv::waitKey(1);
	CloseHandle(hCom);
	return 0;
}										