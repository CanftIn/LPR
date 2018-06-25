#include "RGBPreprocess.h"
#include "common.h"
#include "create_lib.h"
#include "HSVpreprocess.h"
int main(int argc,char *argv[])
{
	feature_lib *ChineseLib;
	feature_lib *AsciiLib;
	int a = 0;
	int select = -1;
	char* cCarFileName[] = {
		"03.bmp",
		"1d0121866ab5255fdec84c76b0b22708.jpg",
		"3a8d340ce1a0565de24a92b1fbcc2447.jpg",
		"9d82c6e047e88b2c729fe1eaccc6f5d6.jpg",
		"ad9a191968cde7c973478541b24868f8.jpg",
		NULL
	};
	char* cPlatImageFileName[] = {
		"1Q169.bmp",
		"268A5.bmp",
		"2P265.bmp",
		"385Y7.bmp",
		"3W532.bmp",
		"5R318.bmp",
		"63T85.bmp",
		"666Z2.bmp",
		"68Z73.bmp",
		"75R32.bmp",
		"767B1.bmp",
		"8G351.bmp",
		"99Q77.bmp",
		"FR111.bmp",
		NULL
	};
	if(argc > 1){
		if(strcmp(argv[1],"c") == 0){
			select=0;
		}
		else if(strcmp(argv[1],"l") == 0){
			select = 1;
		}
		else if(strcmp(argv[1],"r") == 0){
			select = 2;
		}
	}
	else{
		cout << "The number of main function parameters must be 2:" << endl;
		usage();
		return -1;
	}
	
	switch(select)
	{
		case 0:
			cout << "CameraTest..." << endl;
			CameraTest();
		break;
		case 1:
			a=0;
			///**********************车牌定位*********************************/
			
			while(cCarFileName[a] != NULL){
				char path[100];
				Mat srcImage, HSV_Image;
				sprintf(path, "car_image/%s", cCarFileName[a]);
				srcImage = imread(path);
				ShowImage(path,srcImage);
				if(!srcImage.data){
					cout << "read srcImage Image" << "[" << a << "]" <<" faied" << endl;
					return -1;
				}
				
				hsvCheck(srcImage, HSV_Image);
				
				sprintf(path, "%s%d", "HSV",a);
				ShowImage(path, HSV_Image);
				HSV_Filter(HSV_Image);
				sprintf(path, "%s%d", "滤波",a);
				ShowImage(path,HSV_Image);
				MarkerOfOriginal(HSV_Image,srcImage);
				sprintf(path, "%s%d", "标记",a);
				ShowImage(path,srcImage);
				a++;
			}
			///*********************车牌定位*********************************/
		break;
		case 2:
				/*****************建库****************/
			cout << "*****************************************************" << endl;
			cout << "建特征库..." << endl;
			
			InitList(&ChineseLib);
			InitList(&AsciiLib);
			CreateChineseLib(ChineseLib);
			CreateAsciiLib(AsciiLib);
			cout << "建特征库完成" << endl;
			cout << "*****************************************************" << endl;
			/**************建库*********************/
			

			
			
			/*********************字符分割、特征提取与比对**********************/
			
			a=0;
			while (cPlatImageFileName[a] != NULL) {
				char PlatePath[100];
				sprintf(PlatePath, "plate_image/%s", cPlatImageFileName[a]);
				cout << PlatePath << endl;
				Mat srcImage = imread(PlatePath);
				if(!srcImage.data){
					cout << "load plate image" << "[" << a << "] faied" << endl;
					return -1;
				}
				ShowImage("原图", srcImage);

				cvtColor(srcImage, srcImage, CV_BGR2GRAY);
				threshold(srcImage, srcImage, getOstu(srcImage), 255, THRESH_BINARY_INV);
				ShowImage("二值化图", srcImage);
				delRivet(srcImage);
				ShowImage("去除铆钉图", srcImage);

				medianBlur(srcImage, srcImage, 3);
				ShowImage("中值滤波后", srcImage);


				Mat hImage(srcImage.rows, srcImage.cols, CV_8UC1, 255);
				hProjection(srcImage, hImage);
				ShowImage("水平投影", hImage);

				Mat vImage(srcImage.rows, srcImage.cols, CV_8UC1, 255);
				vProjection(srcImage, vImage);
				ShowImage("垂直投影", vImage);

				char WinName[10] = "xihua";
				vector<Mat> SevenWord;
				FindWordEncloseRect(srcImage, hImage, vImage, SevenWord);
				feature_lib Element;
				cout << "\"" << cPlatImageFileName[a] << "\"" << "识别的结果为：" << " ";
				for (int i = 0;i < SevenWord.size();i++) {
					sprintf(WinName, "%d", i);
					threshold(SevenWord[i], SevenWord[i], 128, 255, THRESH_BINARY);
					SevenWord[i] = thinImage(SevenWord[i], -1);
					ShowImage(WinName, SevenWord[i]);
					if (i == 0) {
						MatchFeature(SevenWord[i], ChineseLib, Element);
					}
					else {
						MatchFeature(SevenWord[i], AsciiLib, Element);
					}
					cout << Element.name;
				}
				cout << endl;
				a++;
			}
			DestroyList(ChineseLib);
			DestroyList(AsciiLib);
			/*********************字符分割、特征提取与比对**********************/
		break;
		default:
			cout << "Unknown parameter:" << endl;
			usage();
			return -1;
		break;	
	}
	
	
	
	cvWaitKey(0);
    return 0;
}


