#ifndef __HSVPREPROCESS_H_
#define __HSVPREPROCESS_H_
#include "common.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector> 
using namespace	cv;
using namespace std;
/*
 *函数名称：hsvCheck
 *函数功能：使用颜色作为阈值，将RGB图像转化为HSV二值化图像，
 *			得到指定颜色范围内的区域为白色，其他区域为黑色的图像
 *参数：
 *srcImage：原始的RGB图像
 *dstImage：转化后的HSV二值化图像（输出参数）
 */
void hsvCheck(Mat srcImage,Mat &dstImage);
/*
 *函数名称：HSV_Filter
 *函数功能：对二值化的HSV图像进行滤波
 *参数：
 *HSV_Image：二值化后的HSV图像
 */
void HSV_Filter(Mat &HSV_Image);
/*
 *函数名称：MarkerOfOriginal
 *函数功能：通过二值化后的HSV图像，在RGB原始图像中标记出车牌区域，
			属于车牌颜色区域的部分被标记为红色
 *参数：
 *HSV_Image：二值化后的HSV图像
 *RGB_Original:已标记了车牌区域的图像
 */
void MarkerOfOriginal(const Mat &HSV_Image,Mat &RGB_Original);

#endif

