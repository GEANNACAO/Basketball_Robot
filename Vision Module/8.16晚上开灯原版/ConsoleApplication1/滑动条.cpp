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
	imshow("ԭͼ", asrc);
	//��3��תΪ�Ҷ�ͼ������ͼ��ƽ��
	cvtColor(asrc, midImage, CV_BGR2GRAY);//ת����Ե�����ͼΪ�Ҷ�ͼ
	GaussianBlur(midImage, midImage, Size(5, 5), 2, 2);


//����һЩ����
	namedWindow("��ֵ��ֵ��");
	namedWindow("drawing");
	/*
	// ʹ��Threshold����Ե���ԻҶ�ͼ�������ֵ�����õ���ֵͼ�񣬽�����findContours�ڶ�ֵͼ���ҳ�������
	//createTrackbar("��ֵ", "��ֵ��ֵ��", &value, 255, onThreshold);
	createTrackbar("cmin", "��ֵ��ֵ��", &cmin, 255, onThreshold);
	createTrackbar("cmax", "��ֵ��ֵ��", &cmax, 255, onThreshold);
	*/
	createTrackbar("ilowh", "��ֵ��ֵ��", &ilowh, 179, onThreshold);
	createTrackbar("ihighh", "��ֵ��ֵ��", &ihighh, 179, onThreshold);
	createTrackbar("ilows", "��ֵ��ֵ��", &ilows, 255, onThreshold);
	createTrackbar("ihighs", "��ֵ��ֵ��", &ihighs, 255, onThreshold);
	createTrackbar("ilowv", "��ֵ��ֵ��", &ilowv, 255, onThreshold);
	createTrackbar("ihighv", "��ֵ��ֵ��", &ihighv, 255, onThreshold);


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
	imshow("��ֵ����õ��Ķ�ֵͼ��", threshold_output);

	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));



	//�����������������������������ģ�����������XX��XX֮���������
	vector<vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if ((itc->size()<cmin) || (itc->size()>cmax))      //size�������ĳ���
			itc = contours.erase(itc);
		else
			++itc;
	}


	double a;
	Mat imgROI;
	//��ȡԲ�α߽��
	vector<vector<Point> > contours_poly(contours.size());           //���ڴ�����ߵ㼯�����ƺ�������㼯?????, contours.size()��ʾ����֮ǰҪ��������ĸ���
	//vector<Rect> boundRect( contours.size() );
	vector<Point2f>center(contours.size());                        //��Χ�㼯����СԲ��vector
	vector<float>radius(contours.size());                         //��Χ�㼯����СԲ�ΰ뾶vector
	//һ��ѭ�����������в��֣����б���������ĵĲ���
	for (unsigned int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);//��ָ������3�ƽ����������()���� ��contours_poly[i]������Ľ��Ƶ㼯
		minEnclosingCircle(contours_poly[i], center[i], radius[i]);//�Ը����� 2D�㼯��������approxPolyDP�����ó������ߣ�Ѱ����С����İ�ΧԲ�� 
	}
	
	// ���� Բ�ο�
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
	//imshow("yyy", drawing);
	//int j = 0;
	for (int unsigned i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(255, 255, 160);//���õ���ɫ
		drawContours(drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());//����������ע����Ϊ���ƵĽ����Ǳƽ��Ķ��������
		circle(drawing, center[i], (int)radius[i], color, 2, 8, 0);//����Բ
		//imshow("zzz", drawing);

		a = 3.14*radius[i] * radius[i];
		cout << "Բ������" << center[i] << endl;
		cout << "��İ뾶" << radius[i] << endl;
		cout << "Բ�����" << a << endl;
*/

		
		//��ɫʶ��,�����ֵͼ��ָ����ɫΪ��ɫ
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
		imshow("��ɫʶ��inRange��ֵͼ", imgThresholded);


	/*	if (((center[i].x - radius[i])>0) && ((center[i].y - radius[i]) > 0) &&
		((center[i].x - radius[i]) < 640) && ((center[i].y - radius[i]) < 480))
		{
		imgROI = imgThresholded(Rect((center[i].x - radius[i]), (center[i].y - radius[i]), 2 * radius[i], 2 * radius[i]));  //����Ȥ����Ϊ����Բ��������
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
		cout << i << "Ŀ����" << endl;
		//point = center[i].x;
		}
		else  cout << i << "������" << endl;
		}
		}
		imshow("drawing", drawing);*/
}