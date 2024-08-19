#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

void main()
{
	string path = "C:/Users/user/Desktop/st/3.Camera 2017-05-29 16-23-04_137 [3m3s].avi";
	VideoCapture cap(path);
	Mat frame;
	string path2 = "C:/Users/user/Desktop/education/internship-1/fon.png";
	Mat fon = imread(path2);
	bool firstTime;
	firstTime = true;
	double alpha = 0.9;
	double beta = (1.0 - alpha);
	while (true) {
		cap.read(frame);
		Mat human, tresh, gray_scale;
		vector<vector<Point> > contours;
		vector<vector<int>> detections;

	// вычли текущий кадр из фона
		absdiff(frame, fon, human);
	// 	Настройка порогового значение и поиск контуров человека
		cvtColor(human, gray_scale,  COLOR_BGR2GRAY);
		threshold(gray_scale,tresh,50,255, THRESH_BINARY);
		findContours(tresh, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
	// поиск наибольших контуров среди всех
		vector<vector<Point> > contours_poly(contours.size());
		vector<Rect> boundRect(contours.size());
		double maxArea=0;
		int ind = 0;
		for (size_t i = 0; i < contours.size(); i++)
		{
			// находим максимальную площадь контура и его индекс
			if (contourArea(contours[i]) > maxArea) {
				maxArea = contourArea(contours[i]);
				ind = i;
			}
			approxPolyDP(contours[i], contours_poly[i], 1, true);
			boundRect[i] = boundingRect(contours_poly[i]);
		}
		
		Mat drawing = Mat::zeros(frame.size(), CV_8UC3);
		for (size_t i = 0; i < contours.size(); i++)
		{
			Scalar color = Scalar(0, 255, 0);
			// изображаем только максимальную рамку и отсеиваем маленькие контуры
			if ((i == ind)&&(contourArea(contours[i]) > 100)) {
				rectangle(frame, boundRect[i].tl(), boundRect[i].br(), color, 2);
			}

		}
		imshow("Image", frame);
		waitKey(1);
	}
}

