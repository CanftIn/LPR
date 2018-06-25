#include "create_lib.h"

#include "common.h"
#include "RGBPreprocess.h"
int count_lib;//记录线性表被使用的大小（线性表的空间没被占用一个其值加一
const char* AsciiFileName[] = {
	"0.jpg",
	"1.jpg",
	"2.jpg",
	"3.jpg",
	"4.jpg",
	"5.jpg",
	"6.jpg",
	"7.jpg",
	"8.jpg",
	"9.jpg",
	"A.jpg",
	"B.jpg",
	"C.jpg",
	"D.jpg",
	"E.jpg",
	"F.jpg",
	"G.jpg",
	"H.jpg",
	"J.JPG",
	"K.JPG",
	"L.JPG",
	"M.jpg",
	"N.JPG",
	"P.JPG",
	"Q.JPG",
	"R.JPG",
	"S.JPG",
	"T.JPG",
	"U.JPG",
	"V.jpg",
	"W.JPG",
	"X.JPG",
	"Y.JPG",
	"Z.JPG",
	NULL
};
//获取一个路径下的所有文件名
const char* ChineseFilename[] = {
	"云.jpg",
	"京.jpg",
	"使.jpg",
	"冀.jpg",
	"吉.jpg",
	"学.jpg",
	"宁.jpg",
	"川.jpg",
	"新.jpg",
	"晋.jpg",
	"桂.jpg",
	"沪.jpg",
	"津.jpg",
	"浙.jpg",
	"渝.jpg",
	"港.jpg",
	"湘.jpg",
	"澳.jpg",
	"琼.jpg",
	"甘.jpg",
	"皖.jpg",
	"粤.jpg",
	"苏.jpg",
	"蒙.jpg",
	"藏.jpg",
	"警.jpg",
	"豫.jpg",
	"贵.jpg",
	"赣.jpg",
	"辽.jpg",
	"鄂.jpg",
	"闽.jpg",
	"陕.jpg",
	"青.jpg",
	"领.jpg",
	"鲁.jpg",
	"黑.jpg",
	NULL
};
const char* AsciiWord[] = {
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"J",
	"K",
	"L",
	"M",
	"N",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	NULL
};
const char* ChineseWord[] = {

	"云",
	"京",
	"使",
	"冀",
	"吉",
	"学",
	"宁",
	"川",
	"新",
	"晋",
	"桂",
	"沪",
	"津",
	"浙",
	"渝",
	"港",
	"湘",
	"澳",
	"琼",
	"甘",
	"皖",
	"粤",
	"苏",
	"蒙",
	"藏",
	"警",
	"豫",
	"贵",
	"赣",
	"辽",
	"鄂",
	"闽",
	"陕",
	"青",
	"领",
	"鲁",
	"黑",
	NULL
};



bool create_lib(feature_lib *f_lib,const char* FileName[],const char* WordName[]) {
	vector<float> feature;
	int index = 0;
	while (FileName[index] != NULL) {
		feature.clear();
		char path[256];
		feature_lib TempFeatureLib;
		sprintf(path, "binary24X48/%s", FileName[index]);
		Mat srcImage = imread(path, IMREAD_ANYCOLOR || IMREAD_ANYDEPTH);
		if (!srcImage.data)
		{
			cout << "srcImage " << "[" << index << "]" << " open faied!" << endl;
			cout << path << endl;
			return false;
		}
		
		cvtColor(srcImage, srcImage, CV_BGR2GRAY);
		threshold(srcImage, srcImage, 128, 255, THRESH_BINARY);
		/*if (index == 1)
			cout << srcImage << endl;*/
		srcImage = thinImage(srcImage, -1);
		ShowImage("zizizi", srcImage);
		vector<Mat> ImgCeil;
		ImgCeil.clear();
		DivideMultiRECT(srcImage, DIVIDE_WIDTH, DIVIDE_HIGHT, ImgCeil);
		FeatureSelection(ImgCeil, feature);
		if (FileName[index] == "0.jpg") {
			cout << "0 feature is :" << endl;
			for (int j = 0;j < feature.size();j++) {
				cout << feature[j] << " ";
				
			}
			cout << endl;
			ShowImage("xihua 0", srcImage);
		}
		/*将上述获取的特征值和相应的含义保存到链表中*/
		TempFeatureLib.name = (char *)WordName[index];
		
		for (int j = 0;j < feature.size();j++)
		{
			TempFeatureLib.feature[j] = feature[j];
		}
		
		ListInsert(f_lib, index, &TempFeatureLib);
		index++;

	}
	
	return true;
}

bool CreateChineseLib(feature_lib *ChineseLib) {
	if (!create_lib(ChineseLib, ChineseFilename, ChineseWord)) {
		cout << "CreateChineseLib faied	" << endl;
		return false;
	}
	ListTraverse(ChineseLib);
	return true;
}


bool CreateAsciiLib(feature_lib *AsciiLib) {
	if (!create_lib(AsciiLib, AsciiFileName, AsciiWord)) {
		cout << "CreateChineseLib faied	" << endl;
		return false;
	}
	ListTraverse(AsciiLib);
	return true;
}


