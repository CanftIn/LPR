#include "RGBPreprocess.h"


int getOstu(const Mat &src)
{
	if (!src.data)
	{
		cout << "getOstu faied!" << endl;
		return -1;
	}
	if (src.channels() != 1)
	{
		cout << "输入图像不是单通道灰度图，请转换！<getOstu>" << endl;
		return -1;
	}

	int nl = src.rows;
	int nc = src.cols;
	long size = nl * nc;

	long histogram[256] = { 0 };
	if (src.isContinuous())
	{
		nc = nc * nl;
		nl = 1;
	}
	for (int i = 0; i < nl; ++i)
	{
		const uchar * data = src.ptr<uchar>(i);  
		for (int j = 0; j < nc; j++)
		{
			histogram[int(*data++)]++;
		}
	}
	/*最佳的阈值*/
	int threshold = 0;
	/*存储前景的灰度总和及背景灰度总和*/
	long sum0 = 0, sum1 = 0; 
	/*前景像素的总个数及背景像素的总个数*/
	long cnt0 = 0, cnt1 = 0; 
	/*前景及背景所占整幅图像的比例*/
	double w0 = 0, w1 = 0; 
	/*前景及背景的平均灰度*/
	double u0 = 0, u1 = 0;
	/*最大类间方差*/
	double variance = 0;    

	/*中间变量，用于存储最大间内方差*/
	double maxVariance = 0;
	for (int i = 1; i < 256; i++) //一次遍历每个像素    
	{
		sum0 = 0;
		sum1 = 0;
		cnt0 = 0;
		cnt1 = 0;
		w0 = 0;
		w1 = 0;
		//统计0-i灰度级的像素（假设该灰度范围内为前景）
		for (int j = 0; j < i; j++)
		{
			cnt0 += histogram[j];//前景的总个数
			sum0 += j * histogram[j];//前景的灰度总和
		}

		u0 = (double)sum0 / cnt0;//前景的平均灰度
		w0 = (double)cnt0 / size;//前景所占整幅图像的比例
								 //统计i - 255灰度级的像素（假设该灰度范围内为背景）
		for (int j = i; j <= 255; j++)
		{
			cnt1 += histogram[j];//背景的总个数
			sum1 += j * histogram[j];//背景的灰度总和
		}

		u1 = (double)sum1 / cnt1;//背景的平均灰度
		w1 = 1 - w0; // (double)cnt1 / size;    背景所占整幅图像的比例

		variance = w0 * w1 *  (u0 - u1) * (u0 - u1);
		if (variance > maxVariance)
		{
			maxVariance = variance;
			threshold = i;
		}
	}
	return threshold;
}
void HistEqualize(Mat &srcImage,Mat &dstImage) {
	vector<Mat> channels;
	split(srcImage, channels);
	Mat R, G, B;
	for (int i = 0;i < 3;i++) {
		equalizeHist(channels.at(i), channels.at(i));
	}
	merge(channels, dstImage);
	
}

/************************************投影*****************************************/
/*水平投影*/
bool hProjection(Mat &in, Mat &out)
{
	if (!in.data)
	{
		cout << "open image file is faied !" << endl;
		return false;
	}
	if (in.channels() != 1)
	{
		cout << "原图像不是单通道灰度图像！" << endl;
		return false;
	}
	if (!out.data) {
		cout << "目标图像没有数据" << endl;
		return false;
	}
	if (out.channels() != 1) {
		cout << "目标图像不是单通道灰度图像！" << endl;
		return false;
	}
	int row_number = in.rows, col_number = in.cols;
	int black_num = 0;
	for (int i = 0;i < row_number;i++)
	{
		black_num = 0;
		for (int j = 0;j < col_number;j++)
		{
			if (in.ptr<uchar>(i)[j] == 0)
				black_num++;
		}

		if (black_num < 5)
			continue;
		for (int k = 0;k < black_num;k++)
		{
			out.ptr<uchar>(i)[k] = 0;
		}
	}
	return true;
}

/*垂直投影*/
bool vProjection(Mat &in, Mat &out)
{
	if (!in.data)
	{
		cout << "open image file is faied !" << endl;
		return false;
	}
	if (in.channels() != 1)
	{
		cout << "该图像不是单通道灰度图像！" << endl;
		return false;
	}
	if (!out.data) {
		cout << "目标图像没有数据" << endl;
		return false;
	}
	if (out.channels() != 1) {
		cout << "目标图像不是单通道灰度图像！" << endl;
		return false;
	}
	int row_number = in.rows, col_number = in.cols;
	int black_num = 0;
	//out.create(in.rows, in.cols, CV_8UC1);
	for (int j = 0;j < col_number;j++)
	{
		black_num = 0;
		for (int i = 0;i < row_number;i++)
		{
			if (in.ptr<uchar>(i)[j] == 0)
				black_num++;
		}
		if (black_num < 5)
			continue;
		for (int k = 0;k < black_num;k++)
		{
			out.ptr<uchar>(k)[j] = 0;
		}

	}
	return true;
}

void delRivet(Mat &srcImage) {
	int nl = srcImage.rows;
	int nc = srcImage.cols;
	int iCount = 0;

	for (int i = 0;i < nl;i++) {
		iCount = 0;
		uchar *data = srcImage.ptr<uchar>(i);
		for (int j = 1;j < nc;j++) {
			if (data[j-1]==0 && data[j]==255) {
				iCount++;
			}
		}
		if (iCount < 8) {
			for (int k = 0;k < nc;k++) {
				data[k] = 255;
			}
		}
	}
}




