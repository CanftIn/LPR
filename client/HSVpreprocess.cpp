#include "HSVpreprocess.h"


void hsvCheck(Mat srcImage,Mat &HSV_Image)
{
	Mat imgHSV = Mat::zeros(srcImage.size(), CV_8UC3);
	HSV_Image = Mat::zeros(srcImage.size(), CV_8UC3);
	int iLowH = saturate_cast<uchar>(204 / 2);
	int iHighH = saturate_cast<uchar>(250 / 2);

	int iLowS = saturate_cast<uchar>(35 * 255 / 100);
	int iHighS = saturate_cast<uchar>(255);

	int iLowV = saturate_cast<uchar>(15 * 255 / 100);
	int iHighV = saturate_cast<uchar>(60 * 255 / 100);
	cvtColor(srcImage, imgHSV, COLOR_BGR2HSV);//转为HSV

	inRange(imgHSV, Scalar(iLowH, iLowS, 0), Scalar(iHighH, iHighS, 255), HSV_Image);
}

void HSV_Filter(Mat &HSV_Image){
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3,3));
	morphologyEx(HSV_Image, HSV_Image, MORPH_OPEN, kernel);
}

void MarkerOfOriginal(const Mat &HSV_Image,Mat &RGB_Original){
	int nl = HSV_Image.rows;
	int nc = HSV_Image.cols;
	for (int i = 0;i < nl;i++)
	{
		for (int j = 0;j < nc;j++)
		{
			if (HSV_Image.ptr<uchar>(i)[j] == 255)
			{
				RGB_Original.at<Vec3b>(i, j)[0] = 0;
				RGB_Original.at<Vec3b>(i, j)[1] = 0;
				RGB_Original.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
}
