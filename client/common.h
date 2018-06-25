#ifndef __COMMON_H_
#define __COMMON_H_
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <iomanip>

/**********网络接口头文件*******************/
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <errno.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  


using namespace	cv;
using namespace std;
/************************************************************************/
/* 
维护一个单链表的数据结构，每个节点就是一条信息，每条信息包含的内容如下：
字符名：name
字符特征：feature[9]

维护该数据结构的函数如下：
InitList(Node *pHead)                               :初始化单链表*
DestroyList(Node *pHead)                            :销毁单链表*
ClearList(Node *pHead)                              :清空单链表
ListEmpty(Node *pHead)                              :判断单链表是否为空
ListLength(Node *pHead)                             :获取单链表中节点个数
GetElem(Node *pHead, int index, Node *pElem)        :获取单链表中指定的节点
LocateElem(Node *pHead, Node *pElem)                :给定节点获取单链表中第一次出现的索引位置
ListInsert(Node *pHead, int index, Node *pElem)     :将节点插入单链表中指定位置*
ListDelete(Node *pHead, int index, Node *pElem)     :从单链表中指定位置删除节点*
ListTraverse(Node *pHead)                           :遍历单链表中所有节点*


*/
/************************************************************************/
/*定义分割的区域块数量*/
#define DIVIDE_WIDTH 3
#define DIVIDE_HIGHT 3
#define DIVIDE_ARER (DIVIDE_WIDTH * DIVIDE_HIGHT)

/*定义字符的最小宽度和高度*/

#define MIN_WIDTH 15
#define MIN_HEIGHT 20
typedef struct feature_lib
{
	float feature[DIVIDE_ARER];
	char *name;
	struct feature_lib *pNext;
}feature_lib;

/*初始化单链表*/
bool InitList(feature_lib **pHead);
/*清空单链表*/
void ClearList(feature_lib *pHead);
/*销毁单链表*/
void DestroyList(feature_lib *pHead);
/*判断链表是否为空*/
bool ListEmpty(feature_lib *pHead);
/*获取单链表中节点个数*/
int ListLength(feature_lib *pHead);
/*在链表中比对pElem->feature[9]求平均值，返回最小比对结果对应的节点索引*/
int LocateElem(feature_lib *pHead, feature_lib *pElem);
/*通过节点索引获取相应的节点*/
bool GetElem(feature_lib *pHead, int index, feature_lib *pElem);
/*将节点插入单链表的指定位置*/
bool ListInsert(feature_lib *pHead, int index, feature_lib *pElem);

/*遍历单链表中的所有节点*/
void ListTraverse(feature_lib *plist);
/*************单链表结束**************/

/*
为了便于后续的移植，用该函数完成显示的功能
*/
void ShowImage(char *WinName, Mat srcImage);
/*反转颜色*/
void ReverseColour(Mat &in);
/*
对各个车牌字符归一化大小
*/
bool ResizeImage(Mat &in, Mat &out);
/*将任意图像划分为多个矩形区域，保存到图像向量ceilImg中

*/
void DivideMultiRECT(Mat &srcImage, int CutRows, int CutCols, vector<Mat> &ceilImg);

/*特征提取*/
void FeatureSelection(const vector<Mat> &ceilImg, vector<float> &feature);
/**
* @brief 对输入图像进行细化
* @param src为输入图像,用cvThreshold函数处理过的8位灰度图像格式，元素中只有0与1,1代表有元素，0代表为空白
* @param maxIterations限制迭代次数，如果不进行限制，默认为-1，代表不限制迭代次数，直到获得最终结果
* @return 为对src细化后的输出图像,格式与src格式相同，元素中只有0与1,1代表有元素，0代表为空白
*/
Mat thinImage(const Mat & src, const int maxIterations);

/*
函数功能：根据字符的外切矩形,切割出单个字符图像
参数：in字符图像
	  box字符的外切矩形
	  ROI_iamge保存切割后的字符图像
*/

bool CreateROIMat(Mat &in, Rect box, Mat &ROI_image);
/*找到任意个字符的外切矩形*/
bool FindWordEncloseRect(Mat &srcImage, Mat &hImage, Mat &vImage, vector<Mat> &WordImage);

void MatchFeature(Mat &srcImage, feature_lib *f_lib, feature_lib &element);

void usage();
/*摄像头采集图像*/
void CameraTest();


//待传输图像默认大小为 640*480，可修改  
#define IMG_WIDTH 640   // 需传输图像的宽  
#define IMG_HEIGHT 480  // 需传输图像的高  
#define PACKAGE_NUM 2  
//默认格式为CV_8UC3  
#define BUFFER_SIZE IMG_WIDTH*IMG_HEIGHT*3/PACKAGE_NUM  
/*用于发送视频*/
struct sentbuf  
{  
    char buf[BUFFER_SIZE];
    int flag;
};

class SocketMatTransmissionClient  
{  
public:  
    SocketMatTransmissionClient(void);  
    ~SocketMatTransmissionClient(void);  
  
private:  
    int sockClient;  
    struct sentbuf data;
  
public:  
  
    // 打开socket连接  
    // params : IP      服务器的ip地址  
    //          PORT    传输端口  
    // return : -1      连接失败  
    //          1       连接成功  
    int socketConnect(const char* IP, int PORT);  
  
  
    // 传输图像  
    // params : image 待传输图像  
    // return : -1      传输失败  
    //          1       传输成功  
    int transmit(cv::Mat image);  
  
  
    // 断开socket连接  
    void socketDisconnect(void);  
};  
#endif

