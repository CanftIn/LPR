#include "common.h"

int g_iCount = 0;
void ReverseColour(Mat &in)
{
	if (in.data == NULL)
	{
		return;
	}
	for (int i = 0;i < in.rows;i++)
	{
		for (int j = 0;j < in.cols;j++)
		{
			in.ptr<uchar>(i)[j] = 255 - in.ptr<uchar>(i)[j];
		}
	}
}

/*****************************单链表*************************************/
bool InitList(feature_lib **pHead)
{
	*pHead = (feature_lib *)malloc(sizeof(feature_lib));
	//printf("pList=%p\n", *pHead);
	if (*pHead == NULL)
	{
		return false;
	}
	(*pHead)->pNext = NULL;
	return true;
}

void ClearList(feature_lib *pHead)
{
	feature_lib *pCurrentNode = NULL;
	feature_lib *pNextNode = NULL;
	if (pHead->pNext != NULL)
	{
		pCurrentNode = pHead->pNext;
		while (pCurrentNode != NULL)
		{
			pNextNode = pCurrentNode->pNext;
			free(pCurrentNode);
			pCurrentNode = pNextNode;
		}
	}
}
void DestroyList(feature_lib *pHead)
{
	ClearList(pHead);
	free(pHead);
	pHead = NULL;
}

bool ListEmpty(feature_lib *pHead)
{
	/*方法一
	if (pHead->pNext==NULL)
	{
	return 1;
	}
	else
	{
	return 0;
	}
	*/
	if (g_iCount == 0)
	{
		return true;
	}
	return false;
}
int ListLength(feature_lib *pHead)
{
	return g_iCount;
	/*
	//方法二
	Node *pCurrentNode = pHead->pNext;
	int count = 0;
	while (pCurrentNode != NULL)
	{
	count++;
	pCurrentNode = pCurrentNode->pNext;
	}
	return count;
	*/
}

void ListTraverse(feature_lib *pHead)
{
	feature_lib *pCurrentNode = pHead->pNext;
	while (pCurrentNode != NULL)
	{
		cout << "\"" << pCurrentNode->name << "\"" << "特征值为：" << endl;
		for (int j = 0;j < DIVIDE_ARER;j++)
		{
			cout << pCurrentNode->feature[j] << " ";
		}
		cout << endl << endl;
		pCurrentNode = pCurrentNode->pNext;
	}
}

int LocateElem(feature_lib *pHead, feature_lib *pElem)
{
	int index = 1;
	int ret_index = 0;
	feature_lib *pCurrentNode = pHead->pNext;
	int k = 0;
	float min = 0.0f;
	vector<float> average;
	average.clear();
	while (pCurrentNode != NULL)
	{
		float sum = 0.0f;
		
		for (int j = 0;j < DIVIDE_ARER;j++)
		{
			sum += abs(pCurrentNode->feature[j] - pElem->feature[j]);
		}
		average.push_back(sum / DIVIDE_ARER);
		
		if (k == 0)
		{
			min = average[k];
		}
		if (average[k] < min)
		{
			ret_index = index;
			min = average[k];
			pElem->name = pCurrentNode->name;
		}
		pCurrentNode = pCurrentNode->pNext;
		index++;
		k++;
	}
	return ret_index;
}

bool GetElem(feature_lib *pHead, int index, feature_lib *pElem)
{
	int count = 0;
	if (index<0 || index >g_iCount)
	{
		return false;
	}
	feature_lib *pCurrentNode = pHead->pNext;
	while (pCurrentNode != NULL)
	{
		if (count == index)
		{
			pElem = pCurrentNode;
			return true;
		}
		count++;
		pCurrentNode = pCurrentNode->pNext;
	}
	return true;

}


bool ListInsert(feature_lib *pHead, int index, feature_lib *pElem)
{
	int count = 0;
	feature_lib *pNode = NULL;
	if (index < 0 || index > g_iCount)
	{
		return false;
	}
	pNode = (feature_lib *)malloc(sizeof(feature_lib));
	if (pNode == NULL)
	{
		return false;
	}
	feature_lib *pCurrentNode = pHead;
	if (pNode == NULL)
	{
		return false;
	}
	pNode->name = pElem->name;
	float f;
	for (int j = 0;j < DIVIDE_ARER;j++)
	{
		f = pElem->feature[j];
		pNode->feature[j] = f;
		//cout << pElem->feature[j] << endl;
	}
	while (pCurrentNode != NULL)
	{
		if (count == index)
		{
			//1. 将当前节点的next指针保存
			feature_lib *pTemp = pCurrentNode->pNext;
			//2. 让当前节点的next指针指向申请的内存
			pCurrentNode->pNext = pNode;
			//3. 将保存的next指针赋值给新节点的next指针
			pNode->pNext = pTemp;
			g_iCount++;
			return true;
		}
		count++;
		pCurrentNode = pCurrentNode->pNext;
	}
	return true;
}
bool ListDelete(feature_lib *pHead, int index, feature_lib *pElem)
{
	int count = 0;
	feature_lib *pCurrentNode = pHead;
	feature_lib *pPreNode = NULL;
	if (index <= 0 || index > g_iCount)
	{
		return false;
	}

	while (pCurrentNode != NULL)
	{
		if (count == index)
		{

			//1. 使currentNode的上一个节点指向currentNode的下一个节点
			pPreNode->pNext = pCurrentNode->pNext;

			if (pElem != NULL)
			{
				//将要删除的节点数据拷贝出来
				strcpy(pElem->name, pCurrentNode->name);
				for (int j = 0;j < 9;j++)
				{
					pElem->feature[j] = pElem->feature[j];
				}
			}


			//2. 删除currentNode指向的节点
			free(pCurrentNode);
			g_iCount--;
			return true;
		}
		count++;
		pPreNode = pCurrentNode;
		pCurrentNode = pCurrentNode->pNext;
	}
	return true;
}
/***********************************单链表************************************************/



/***************************************图像显示函数*****************************************/
/*
为了便于后续的移植，用该函数完成显示的功能
*/
void ShowImage(char *WinName,Mat srcImage) {
	imshow(WinName, srcImage);
}

/************************************图像显示函数结束****************************************/


/***********************************特征提取部分*******************************************/




/*根据字符矩形从切割出字符返回到ROI_image*/
bool CreateROIMat(Mat &in, Rect box, Mat &ROI_image)
{
	if (in.data == NULL)
	{
		cout << "error:in.data=null" << endl;
		return false;
	}
	ROI_image.create(box.height, box.width, CV_8UC1);
	ROI_image = in(box);
	return true;
}
/***********************************特征提取部分*******************************************/


/*对各个车牌字符归一化大小*/
bool ResizeImage(Mat &in, Mat &out)
{
	if (in.data == NULL)
	{
		return false;
	}
	if (in.channels() != 1)
	{
		cout << "不能对图像进行归一化尺寸，该图像不是单通道！" << endl;
		return false;
	}
	Mat ds = imread("binary24X48/0.jpg", IMREAD_ANYCOLOR || IMREAD_ANYDEPTH);
	out = ds.clone();
	resize(in, out, out.size(), 0.0, 0.0, INTER_LINEAR);
	return true;
}
/*找到字符的外切矩形，可实现找到任意个字符*/
bool FindWordEncloseRect(Mat &srcImage, Mat &hImage, Mat &vImage,  vector<Mat> &ResizeWordImage)
{
	bool begin = false;
	bool bIsWord = false;
	/*记录xy的起始位置*/
	int x_start = 0, x_end = 0;
	int y_start = 0, y_end = 0;
	/*记录外切矩形的宽高*/
	int width = 0;
	int height = 0;
	if (!vImage.data) {
		return false;
	}
	if (vImage.type() != CV_8UC1) {
		return false;
	}
	int i = 1;
	
	/*temp_rect用于暂存字符外切矩形的信息*/
	Rect temp_rect;
	/*RoiImage用于暂存字符外切矩形的信息（未规格化字符图像的大小）*/
	Mat RoiImage;
	/*temp_img用于暂存规格化大小的图像*/
	Mat temp_img;
	/*WordRect用于保存字符外切矩形的信息*/
	vector<Rect> WordRect;
	/*WordImage用于保存切割出来的字符（还为规格化大小）*/
	vector<Mat> WordImage;
	WordRect.clear();
	WordImage.clear();
	ResizeWordImage.clear();
	/*根据水平投影找到字符的高，假设字符的最小高度为10*/
	do {
		bIsWord = false;
		begin = false;
		for (;i < hImage.rows;i++) {
			if ((hImage.ptr<uchar>(i - 1)[0] == 255) && (hImage.ptr<uchar>(i)[0] == 0) && !begin) {
				begin = true;
				y_start = i;
			}
			if ((hImage.ptr<uchar>(i - 1)[0] == 0) && (hImage.ptr<uchar>(i)[0] == 255) && begin) {
				y_end = i;
				height = y_end - y_start;
				begin = false;
				break;
			}
			if (height > MIN_HEIGHT) {
				bIsWord = true;
			}
			else {
				bIsWord = false;
			}
		}
	} while (!bIsWord || i < hImage.rows);

	i = 0;

	int k = 0;
	do {
		bIsWord = false;
		begin = false;
		width = 0;
		for (;i < vImage.cols;i++) {
			if (!begin && (vImage.ptr<uchar>(0)[i - 1] == 255) && (vImage.ptr<uchar>(0)[i] == 0)) {
				begin = true;
				x_start = i;
			}
			if ((vImage.ptr<uchar>(0)[i - 1] == 0) && (vImage.ptr<uchar>(0)[i] == 255) && begin) {
				begin = false;
				x_end = i;
				width = x_end - x_start;
				break;
			}

		}
		if (width > 8) {
			temp_rect.height = height;
			temp_rect.y = y_start;
			temp_rect.x = x_start;
			temp_rect.width = width;
			WordRect.push_back(temp_rect);
			k++;
		}
	} while (i < vImage.cols);
	
	vector<Rect>::iterator it;
	for (it = WordRect.begin();it != WordRect.end();it++) {
		CreateROIMat(srcImage, *it, RoiImage);
		WordImage.push_back(RoiImage);
	}
	
	
	char WinName[10] = "Win";
	k = 0;
	vector<Mat>::iterator m_it;
	
	for (m_it = WordImage.begin();m_it != WordImage.end();m_it++)
	{
		/*
		如果宽度小于MIN_WIDTH。首先判断，
		图像的垂直方向上的黑像素（前景像素）的个数是否小于图像高度的一半，
		若小于可判定它是字符之间的黑点，将其过滤；
		若不小于，则这个图像是字符1，扩展该字符的宽度，使得与其他字符图像拥有相同的分辨率
		*/
		if ((m_it->cols < MIN_WIDTH)) {
			int BlackCount = 0;
			int mid = (m_it->cols) / 2;
			for (i = 0;i < m_it->rows;i++) {
				if (m_it->ptr<uchar>(i)[mid] == 0) {
					BlackCount++;
				}
			}
			if (BlackCount < (m_it->rows)) {
				continue;
			}
			int iExtend = (WordImage[0].cols - m_it->cols) / 2;
			copyMakeBorder(*m_it, *m_it, 0, 0, iExtend, iExtend, 0, 255);
		}
		ResizeImage(*m_it, temp_img);
		ResizeWordImage.push_back(temp_img);
		sprintf(WinName, "[%d]", k);
		ShowImage(WinName, ResizeWordImage[k]);
		k++;
	}
	return true;
}


/*对字符进行细化*/
Mat thinImage(const Mat & src, const int maxIterations)
{
	assert(src.type() == CV_8UC1);
	Mat dst;
	int width = src.cols;
	int height = src.rows;
	src.copyTo(dst);
	int count = 0;  //记录迭代次数  
	threshold(dst, dst, 128, 1, THRESH_BINARY_INV);

	//ShowImage("src", src);
	while (true)
	{
		count++;
		if (maxIterations != -1 && count > maxIterations) //限制次数并且迭代次数到达  
			break;
		std::vector<uchar *> mFlag; //用于标记需要删除的点  
									//对点标记  
		for (int i = 0; i < height;++i)
		{
			uchar * p = dst.ptr<uchar>(i);
			for (int j = 0; j < width; ++j)
			{
				//如果满足四个条件，进行标记  
				//  p9 p2 p3  
				//  p8 p1 p4  
				//  p7 p6 p5  
				uchar p1 = p[j];
				if (p1 != 1) continue;
				uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
				uchar p8 = (j == 0) ? 0 : *(p + j - 1);
				uchar p2 = (i == 0) ? 0 : *(p - dst.step + j);
				uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - dst.step + j + 1);
				uchar p9 = (i == 0 || j == 0) ? 0 : *(p - dst.step + j - 1);
				uchar p6 = (i == height - 1) ? 0 : *(p + dst.step + j);
				uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + dst.step + j + 1);
				uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + dst.step + j - 1);
				if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
				{
					int ap = 0;
					if (p2 == 0 && p3 == 1) ++ap;
					if (p3 == 0 && p4 == 1) ++ap;
					if (p4 == 0 && p5 == 1) ++ap;
					if (p5 == 0 && p6 == 1) ++ap;
					if (p6 == 0 && p7 == 1) ++ap;
					if (p7 == 0 && p8 == 1) ++ap;
					if (p8 == 0 && p9 == 1) ++ap;
					if (p9 == 0 && p2 == 1) ++ap;

					if (ap == 1 && p2 * p4 * p6 == 0 && p4 * p6 * p8 == 0)
					{
						//标记  
						mFlag.push_back(p + j);
					}
				}
			}
		}

		//将标记的点删除  
		for (std::vector<uchar *>::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
		{
			**i = 0;
		}

		//直到没有点满足，算法结束  
		if (mFlag.empty())
		{
			break;
		}
		else
		{
			mFlag.clear();//将mFlag清空  
		}

		//对点标记  
		for (int i = 0; i < height; ++i)
		{
			uchar * p = dst.ptr<uchar>(i);
			for (int j = 0; j < width; ++j)
			{
				//如果满足四个条件，进行标记  
				//  p9 p2 p3  
				//  p8 p1 p4  
				//  p7 p6 p5  
				uchar p1 = p[j];
				if (p1 != 1) continue;
				uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
				uchar p8 = (j == 0) ? 0 : *(p + j - 1);
				uchar p2 = (i == 0) ? 0 : *(p - dst.step + j);
				uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - dst.step + j + 1);
				uchar p9 = (i == 0 || j == 0) ? 0 : *(p - dst.step + j - 1);
				uchar p6 = (i == height - 1) ? 0 : *(p + dst.step + j);
				uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + dst.step + j + 1);
				uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + dst.step + j - 1);

				if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
				{
					int ap = 0;
					if (p2 == 0 && p3 == 1) ++ap;
					if (p3 == 0 && p4 == 1) ++ap;
					if (p4 == 0 && p5 == 1) ++ap;
					if (p5 == 0 && p6 == 1) ++ap;
					if (p6 == 0 && p7 == 1) ++ap;
					if (p7 == 0 && p8 == 1) ++ap;
					if (p8 == 0 && p9 == 1) ++ap;
					if (p9 == 0 && p2 == 1) ++ap;

					if (ap == 1 && p2 * p4 * p8 == 0 && p2 * p6 * p8 == 0)
					{
						//标记  
						mFlag.push_back(p + j);
					}
				}
			}
		}

		//将标记的点删除  
		for (std::vector<uchar *>::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
		{
			**i = 0;
		}

		//直到没有点满足，算法结束  
		if (mFlag.empty())
		{
			break;
		}
		else
		{
			mFlag.clear();//将mFlag清空  
		}

	}
	for (int i = 0;i < height;i++)
	{
		for (int j = 0;j < width;j++)
		{
			if (dst.ptr<uchar>(i)[j] == 1)
			{
				dst.ptr<uchar>(i)[j] = 0;
			}
			else
				dst.ptr<uchar>(i)[j] = 255;
		}
	}
	return dst;
}

/*将字符矩形分为多个区域*/
void DivideMultiRECT(Mat &srcImage,int CutRows, int CutCols, vector<Mat> &ceilImg)
{
	int height = srcImage.rows;
	int width = srcImage.cols;

	int ceil_height = (int)(height / CutRows);
	int ceil_width = (int)(width / CutCols);
	int ceil_down_height = height - (CutRows - 1) * ceil_height;
	int ceil_right_width = width - (CutCols - 1) * ceil_width;
	//Mat hImage(srcImage.rows, srcImage.cols, CV_8UC1, 255);
	
	for (int i = 0; i < CutRows - 1; i++) {
		for (int j = 0; j < CutCols; j++) {
			if (j < CutCols - 1) {
				Rect rect(j*ceil_width, i*ceil_height, ceil_width, ceil_height);
				ceilImg.push_back(srcImage(rect));

			}
			else {
				Rect rect((CutCols - 1)*ceil_width, i*ceil_height, ceil_right_width, ceil_height);
				ceilImg.push_back(srcImage(rect));
			}
		}
	}
	for (int i = 0; i<CutCols; i++)
	{
		if (i<CutCols - 1)
		{
			Rect rect(i*ceil_width, (CutRows - 1)*ceil_height, ceil_width, ceil_down_height);
			ceilImg.push_back(srcImage(rect));
		}
		else   //右下角这个图像块  
		{
			Rect rect((CutCols - 1)*ceil_width, (CutRows - 1)*ceil_height, ceil_right_width, ceil_down_height);
			ceilImg.push_back(srcImage(rect));
		}
	}
}


/*特征提取*/
void FeatureSelection(const vector<Mat> &ceilImg, vector<float> &feature) {
	int k = 0;
	int BlackNum = 0;
	float temp;
	while (k < ceilImg.size()) {
		BlackNum = 0;
		for (int i = 0;i < ceilImg[k].rows;i++) {
			for (int j = 0;j < ceilImg[k].cols;j++) {
				if (ceilImg[k].ptr<uchar>(i)[j] == 0) {
					BlackNum++;
				}
			}
		}
		temp = (float)BlackNum / ((ceilImg[k].rows) * (ceilImg[k].cols) *1.0f);
		feature.push_back(temp);
		k++;
	}
}

void MatchFeature(Mat &srcImage, feature_lib *f_lib,feature_lib &element) {
	vector<float> feature;
	vector<Mat> ImgCeil;
	if (ListEmpty(f_lib)) {
		return;
	}
	ImgCeil.clear();
	
	ShowImage("", srcImage);
	DivideMultiRECT(srcImage, DIVIDE_WIDTH, DIVIDE_HIGHT, ImgCeil);
	FeatureSelection(ImgCeil, feature);
	//cout << "----------------------------from MatchFeature-------------------------" << endl;
	//cout << "feature的大小" << feature.size() << endl;
	for (int j = 0;j < feature.size();j++)
	{
		element.feature[j] = feature[j];
		//cout << element.feature[j] << " ";
	}
	//cout << endl;
	//ShowImage("srcImage", srcImage);
	LocateElem(f_lib, &element);
}

  
SocketMatTransmissionClient::SocketMatTransmissionClient(void)  
{
}
SocketMatTransmissionClient::~SocketMatTransmissionClient(void)  
{  
}  
  
  
int SocketMatTransmissionClient::socketConnect(const char* IP, int PORT)  
{  
    struct sockaddr_in    servaddr;  
  
    if ((sockClient = socket(AF_INET, SOCK_STREAM, 0)) < 0)   
    {  
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);  
        return -1;  
    }  
  
    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_port = htons(PORT);  
    if (inet_pton(AF_INET, IP, &servaddr.sin_addr) <= 0)   
    {  
        printf("inet_pton error for %s\n", IP);  
        return -1;  
    }  
  
    if (connect(sockClient, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)   
    {  
        printf("connect error: %s(errno: %d)\n", strerror(errno), errno);  
        return -1;  
    }  
    else   
    {  
        printf("connect successful!\n");  
    }  
}  
  
  
void SocketMatTransmissionClient::socketDisconnect(void)  
{  
    close(sockClient);  
}  
  
int SocketMatTransmissionClient::transmit(cv::Mat image)  
{
	int row_number = image.rows;
	int col_number = image.cols * 3;
    if (image.empty())  
    {  
        printf("empty image\n\n");  
        return -1;  
    }
    if(image.cols != IMG_WIDTH || image.rows != IMG_HEIGHT || image.type() != CV_8UC3)  
    {  
        printf("the image must satisfy : cols == IMG_WIDTH（%d）  rows == IMG_HEIGHT（%d） type == CV_8UC3\n\n", IMG_WIDTH, IMG_HEIGHT);  
        return -1;  
    }
	
    for(int k = 0; k < PACKAGE_NUM; k++)   
    {  
        int num1 = IMG_HEIGHT / PACKAGE_NUM * k;  
        for (int i = 0; i < IMG_HEIGHT / PACKAGE_NUM; i++)  
        {  
            int num2 = i * IMG_WIDTH * 3;  
            uchar* ucdata = image.ptr<uchar>(i + num1);  
            for (int j = 0; j < IMG_WIDTH * 3; j++)  
            {  
                data.buf[num2 + j] = ucdata[j];  
            }  
        }
        if(k == PACKAGE_NUM - 1)  
            data.flag = 2;  
        else  
            data.flag = 1;  
  
        if (send(sockClient, (char *)(&data), sizeof(data), 0) < 0)  
        {  
            printf("send image error: %s(errno: %d)\n", strerror(errno), errno);  
            return -1;  
        }  
    }
	char message[1];
	do{
		if(recv(sockClient, message, 1, 0) < 0){
			cout << "client recv failed:" << endl;
		}
	}while(strcmp(message,"r"));
	
}


void CameraTest()  
{
	VideoCapture capture(0);
	Mat image;
	char r[1];
	SocketMatTransmissionClient socketMat; 
	if (socketMat.socketConnect("127.0.0.1", 6666) < 0)  
	{  
		return;  
	}  
	
	while (1)  
	{  
		if (!capture.isOpened()){
			cout << "error:camera isOpened" << endl;
			return;
		}
		capture >> image;
		
		socketMat.transmit(image);
		
	}
	socketMat.socketDisconnect();
}


void usage(){
	cout << "****************************************************"<<endl;
	cout << "usage:" << endl;
	cout << "argv[0]: arm_client" << endl;
	cout << "argv[1]: c or l or r" << endl;
	cout << "\"c\" is used to test the camera" << endl;
	cout << "\"l\" is used to test locate the plate" << endl;
	cout << "\"r\" is used to test recognition" << endl;
	cout << "****************************************************"<<endl;
}


