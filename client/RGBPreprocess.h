#ifndef __RGBPREPROCESS_H_
#define __RGBPREPROCESS_H_
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector> 
#include "common.h"
#define TARGET_IS_BLACK 0
#define TARGET_IS_WHITE 1
using namespace	cv;
using namespace std;

/*
函数功能：基于OS-TU的自适应二值化分割阈值的确定
参数说明：
src单通道灰度图像
函数返回值：
成功返回最佳阈值（非负）
失败返回-1
*/
int getOstu(const Mat &src);
/*
直方图均衡化
*/
void HistEqualize(Mat &srcImage, Mat &dstImage);
/*去除铆钉*/
void delRivet(Mat &srcImage);

/*水平投影*/
bool hProjection(Mat &in, Mat &out);
/*垂直投影*/
bool vProjection(Mat &in, Mat &out);

#endif
