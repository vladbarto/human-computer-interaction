#include <iostream>
#include <opencv2/opencv.hpp>
#include "common.h"
#include "Lab 1 - Spatii de culoare/lab1.h"
#include "Lab 2 - Clasificarea pixelilor pe baza modelului/lab2.h"

using namespace std;
using namespace cv;

std::string main_pictures_path = std::string(MAIN_IMAGE_FOLDER_PATH);

void testOpenImage() {
	std::string temp = main_pictures_path;
	temp.append("/A1.bmp");
	openImage(temp.c_str());
}

void testParcurgereSimplaDiblookStyle()
{
	std::string fname;
	while (!(fname = openFileDlg()).empty())
	{
		Mat src = imread(fname.c_str(),
			IMREAD_GRAYSCALE);//CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		int w = src.step; // no dword alignment is done !!!
		Mat dst = src.clone();

		double t = (double)getTickCount(); // Get the current time [s]

		// the fastest approach using the �diblook style�
		uchar *lpSrc = src.data;
		uchar *lpDst = dst.data;
		for (int i = 0; i<height; i++)
			for (int j = 0; j < width; j++) {
				uchar val = lpSrc[i*w + j];
				lpDst[i*w + j] = 255 - val;
				/* sau puteti scrie:
				uchar val = lpSrc[i*width + j];
				lpDst[i*width + j] = 255 - val;
				//	w = width pt. imagini cu 8 biti / pixel
				//	w = 3*width pt. imagini cu 24 biti / pixel
				*/
			}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms]
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image", src);
		imshow("negative image", dst);
		waitKey();
	}
}
void nop() {;}
int main()
{
	int op;
	do
	{
		system("clear");
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 1 - Open image\n");
		printf(" 2 - Open BMP images from folder\n");
		printf(" 3 - Image negative - diblook style\n");
		printf(" 4 - BGR->HSV\n");
		printf(" 5 - Resize image\n");
		printf(" 6 - Canny edge detection\n");
		printf(" 7 - Edges in a video sequence\n");
		printf(" 8 - Snap frame from live video\n");
		printf(" 9 - Mouse callback demo\n");
		printf("-------lab1-------\n");
		printf("11 - My BGR to HSV + histograme \n");
		printf("14 - Binarizare componenta H, prag arbitrar ales \n");
		printf("15 - Binarizare componenta H, globala automata \n");
		printf("16 - list hist \n");
		printf("21 - Lab 2 \n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
			case 1:
				testOpenImage();
				break;
			case 2:
				testOpenImage();
				break;
			case 3:
				testParcurgereSimplaDiblookStyle(); //diblook style
				break;
		// 	case 4:
		// 		//testColor2Gray();
		// 		testBGR2HSV();
		// 		break;
		// 	case 5:
		// 		testResize();
		// 		break;
		// 	case 6:
		// 		testCanny();
		// 		break;
		// 	case 7:
		// 		testVideoSequence();
		// 		break;
		// 	case 8:
		// 		testSnap();
		// 		break;
		// 	case 9:
		// 		testMouseClick();
		// 		break;
			case 11:
				testMyBGR2HSV();
				break;
			case 14:
				testBinarizareManualaPragUnic();
				break;
			case 15:
				testBinarizareGlobalaAutomata();
				break;
			case 16:
				setMouseCallback("Fereastra", MyCallBackFuncHSV);
				testMouseClick();
				break;
			case 21:
				L2_work_Build();
				break;

		}
	}
	while (op!=0);
	return 0;
}
