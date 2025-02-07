#include <fstream>
#include <opencv2/opencv.hpp>
#include "common.h"
#include "Lab 1 - Spatii de culoare/lab1.h"
#include "Lab 2 - Clasificarea pixelilor pe baza modelului/lab2.h"
#include "Lab 3 - Segmentarea bazata pe regiuni/lab3.h"
#include "Lab 4 - Detectia colturilor/lab4.h"
#include "Lab 5 - Segmentarea obiectelor in miscare prin background substraction/lab5.h"
#include "Lab_6_Estimarea_fluxului_optic/lab6.h"
#include "Lab_7_Estimarea_fluxului_optic_dens/lab7.h"
#include "Lab_8_Faces/lab8.h"
#include "Lab_9_Detectie_Faciala/lab9.h"
#include "Lab_10_Detectia_de_persoane/lab10.h"
#include "Lab_11/lab11.h"

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

void testVideoSequence()
{
	//VideoCapture cap("./Videos/rubic.avi"); // off-line video from file
	VideoCapture cap(0);	// live video from web cam
	if (!cap.isOpened()) {
		printf("Cannot open video capture device.\n");
		waitKey();
		return;
	}

	Mat edges;
	Mat frame;
	char c;

	while (cap.read(frame))
	{
		Mat grayFrame;
		cvtColor(frame, grayFrame, COLOR_BGR2GRAY);
		Canny(grayFrame,edges,40,100,3);
		imshow("source", frame);
		imshow("gray", grayFrame);
		imshow("edges", edges);
		c = waitKey();  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished\n");
			break;  //ESC pressed
		};
	}
}

void testOpenImagesFld()
{
	char folderName[MAX_PATH];
	if (openFolderDlg(folderName)==0)
		return;
	char fname[MAX_PATH];
	FileGetter fg(folderName,"bmp");
	// printf("gets here");

	while(fg.getNextAbsFile(fname))
	{
		Mat src;
		src = imread(fname);
		imshow(fg.getFoundFileName(),src);
		if (waitKey()==27) //ESC pressed
			break;
	}
}

void nop() {;}

int main()
{
	makeColorwheel(); // initaializes the colorwhel for the colorcode module
	make_HSI2RGB_LUT();
	int op;
	do
	{
		//system("clear");
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
		printf("-------lab2-------\n");
		printf("21 - Lab 2 \n");
		printf("-------lab3-------\n");
		printf("31 - Lab 3 - Region Growing Color (Hue) \n");
		printf("32 - Lab 3 - Region Growing Grayscale (Value) \n");
		printf("-------lab4-------\n");
		printf("41 - Lab 4 - goodFeaturesToTrack() \n");
		printf("44 - Lab 4 - call cornerHarris_demo() \n");
		printf("45 - Lab 4 - video sequence corner detection (function from cmd 41) \n");
		printf("-------lab5-------\n");
		printf("51 - Lab 5 - Segmentarea obiectelor in miscare, background substraction \n");
		printf("-------lab6-------\n");
		printf("61 - Lab 6 -  \n");
		printf("-------lab7-------\n");
		printf("71 - Lab 7 -  \n");
		printf("-------lab8-------\n");
		printf("81 - Lab 8 - Detectia fețelor pe imagini \n");
		printf("82 - Lab 8 - Detectia fețelor pe video (haar) \n");
		printf("83 - Lab 8 - Detectia fețelor pe video (lbp) \n");
		printf("-------lab9-------\n");
		printf("91 - Lab 9 - Validarea modificarilor pe detectia faciala (clipit) \n");
		printf("-------lab10-------\n");
		printf("101 - Lab 10 - Detectia persoanelor (full body) \n");
		printf("-------lab11-------\n");
		printf("111 - Lab 11 -  \n");
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
			case 7:
				testVideoSequence();
				break;
		// 	case 8:
		// 		testSnap();
		// 		break;
			// case 9:
				// testMouseClick();
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

			case 31:
				L3_ColorModel_Build_Color();
				break;
			case 32:
				L3_ColorModel_Build_Grayscale();
				break;

			case 41:
				cornerDetectionDemo();
				break;
			case 44:
				callCornerHarrisDemo();
				break;
			case 45:
				videoSequenceCornerDetection();
				break;

			case 51:
				lab5_background_substraction();
				break;

			case 61:
				testOpticalFlow2();
				break;

			case 71:
				farnebackOpticalFlowDemo();
				break;

			case 81:
				testFaceDetectAndDisplay_Images();
				break;
			case 82:
				testFaceDetectAndDisplay_Video_Haar();
				break;
			case 83:
				testFaceDetectAndDisplay_Video_LBP();
				break;

			case 91:
				validareDetectie();
				break;

			case 101:
				detectPeople();
				break;

			case 111:
				testBodyDetectionHog();
				break;
			default:
				nop();
		}
	}
	while (op!=0);
	return 0;
}
