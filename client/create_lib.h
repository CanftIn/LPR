#ifndef __CREATE_LIB_H_
#define __CREATE_LIB_H_
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include "common.h"
using namespace std;
using namespace cv;
/*
函数功能：根据的字符图像的文件名，实现将中文字符和ASCII字符分别建库
参数：f_lib为字符特征库容器的指针，
	 FileName为所有的汉字字符图像名称或ASCII字符图像名称
	 WordName为所有的汉字字符的名称或ASCII字符的名称
返回值：成功返回true，失败返回false
*/
bool create_lib(feature_lib *f_lib, char *FileName[], char *WordName[]);
/*
函数功能：构建中文字符特征库，该函数实际调用create_lib
参数：ChineseLib为汉字特征库容器的指针
返回值：成功返回true，失败返回false
*/
bool CreateChineseLib(feature_lib *ChineseLib);
/*
函数功能：构建0-9，A-Z字符特征库，该函数实际调用create_lib
参数：AsciiLib为数字或字母特征库容器的指针
返回值：成功返回true，失败返回false
*/
bool CreateAsciiLib(feature_lib *AsciiLib);

//

#endif


