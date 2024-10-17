#include <iostream>
#include <opencv2/opencv.hpp>
#include "common.h"
#include "Lab 1 - Spatii de culoare/lab1.h"
#include "Lab 2 - Clasificarea pixelilor pe baza modelului/lab2.h"
#include <queue>
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

//Functia CallBack care se apeleaza la declansarea evenimentelor de nmouse
void CallBackFuncL3(int event, int x, int y, int flags, void* userdata)
{
	float konst = 2.5;
	float T = konst * hue_std;

	float hue_avg;
    Mat* H = (Mat*)userdata;
    Rect roi; // regiunea de interes curenta (ROI)
    if (event == EVENT_LBUTTONDOWN) // se alege punctul de start
    {
        // punctul de start al ROI
        Pstart.x = x;
        Pstart.y = y;
        draw = true;
        printf("Pstart: (%d, %d) \n ", Pstart.x, Pstart.y);

		int nr = 0;
    	for(int neigh_row = x - 1; neigh_row <= x + 1; neigh_row++) {
    		for(int neigh_col = y - 1; neigh_col <= y + 1; neigh_col++) {
    			if(neigh_row < 0 || neigh_row > (*H).rows || // verific ca coordonatele sa fie in dimensiunile lui H
					neigh_col < 0 || neigh_col > (*H).cols)
    				continue;
    			else {
    				hue_avg += (*H).at<uchar>(neigh_row, neigh_col);
    				nr++;
    			}
    		}
    	}
    	hue_avg /= (float)nr;
    }
    else if (event == EVENT_MOUSEMOVE)
    {
        ; // NoOP
    }
    else if (event == EVENT_LBUTTONUP) // am ales punctul de start, hai acum sa implementez region growing
    {
    	Mat labels = Mat::zeros((*H).size(),CV_8UC1);
    	queue <Point> que;
    	int k3=255; //ethiceta curenta (direct alb)
    	int N3=1; // numarul de pixeli din regiune

    	que.push(Point(x,y)); // adauga element (seed point) in coada
    	// acesta primeste eticheta k

    	while (!que.empty())
    	{ // Retine poz. celui mai vechi element din coada
    		Point oldest=que.front();
    		que.pop(); // scoate element din coada
    		int xx=oldest.x; // coordonatele lui
    		int yy=oldest.y;

    		// Pentru fiecare vecin al pixelului (xx, yy) ale carui coordonate sunt in interiorul imaginii
    		for(int neigh_row = xx - 1; neigh_row <= xx + 1; neigh_row++) {
    			for(int neigh_col = yy - 1; neigh_col <= yy + 1; neigh_col++) {
    				if(neigh_row < 0 || neigh_row > (*H).rows || // verific ca coordonatele sa fie in dimensiunile lui H
    					neigh_col < 0 || neigh_col > (*H).cols)
    					continue;
    				// else
    				uchar hue_vecin = (*H).at<uchar>(neigh_row, neigh_col);
    				uchar hue = (*H).at<uchar>(xx, yy);

    				// Daca abs(hue(vecin) – Hue_avg)<T si labels(vecin) == 0
    				if((abs(hue_vecin) - hue_avg) < T && labels.at<uchar>(neigh_row, neigh_col) == 0) {

    					// Aduga vecin la regiunea curenta
						Point point2add = Point(neigh_row, neigh_col);
    					que.push(point2add);

    					// labels(vecin) = k
						labels.at<uchar>(neigh_row, neigh_col) = k3;

    					// Actualizeaza Hue_avg (medie ponderata)
    					hue_avg = (N3*hue_avg + hue)/(N3+1);

    					// Incrementeza N
    					N3++;
    				}
    			}
    		}

    	}

    	Mat element1 = getStructuringElement(MORPH_CROSS, Size(3, 3));
    	// crearea unui element structural de dimensiune 3x3 de tip patrat (V8)
    	Mat element2 = getStructuringElement(MORPH_RECT, Size(3, 3));

    	// dilatare cu acest element structural (aplicata 2x)
    	dilate(labels, labels, element2, Point(-1, -1), 2);
	    //
    	// //eroziune cu acest element structural (aplicata 1x)
    	erode(labels, labels, element1, Point(-1, -1), 1);



		imshow("Labels", labels);
    	waitKey(27);
    }
}

// Functia principala in care se citesc imaginile sursa si se face legatura intre
// functia de mouse CallBack si fereastra in caare este afisata imaginea sursa
void L3_ColorModel_Build_Color()
{
    //Mat srcg; declarata global
    Mat hsv;
    // Citestea imaginea din fisier
    std::string fname;
    while (!(fname = openFileDlg()).empty())
    {
        srcg = imread(fname.c_str());
        int height = srcg.rows;
        int width = srcg.cols;
        // Aplicare FTJ gaussian pt. eliminare zgomote / netezire imagine
        // http://opencvexamples.blogspot.com/2013/10/applying-gaussian-filter.html
        GaussianBlur(srcg, srcg, Size(5, 5), 0, 0);

        //Creare fereastra pt. afisare
        namedWindow("src", 1);

        // Componenta de culoare Hue a modelului HSV
        Mat channels[3];
        cvtColor(srcg, hsv, COLOR_BGR2HSV); // conversie RGB -> HSV
        split(hsv, channels);

        // Componentele de culoare ale modelului HSV
        Mat H = channels[0] * 255 / 180;
        Mat S = channels[1];
        Mat V = channels[2];

        // asociere functie de tratare a evenimentelor MOUSE cu ferestra "src"
        // Ultimul parametru al functiei este adresa lamatricea H
        setMouseCallback("src", CallBackFuncL3, &H);
        imshow("src", srcg);

        // Wait until user press some key
        waitKey(0);
    }
}

void L3_ColorModel_Build_Grayscale()
{
	//Mat srcg; declarata global
	Mat hsv;
	// Citestea imaginea din fisier
	std::string fname;
	while (!(fname = openFileDlg()).empty())
	{
		srcg = imread(fname.c_str());
		int height = srcg.rows;
		int width = srcg.cols;
		// Aplicare FTJ gaussian pt. eliminare zgomote / netezire imagine
		// http://opencvexamples.blogspot.com/2013/10/applying-gaussian-filter.html
		GaussianBlur(srcg, srcg, Size(5, 5), 0, 0);

		//Creare fereastra pt. afisare
		namedWindow("src", 1);

		// Componenta de culoare Hue a modelului HSV
		Mat channels[3];
		cvtColor(srcg, hsv, COLOR_BGR2HSV); // conversie RGB -> HSV
		split(hsv, channels);

		// Componentele de culoare ale modelului HSV
		Mat H = channels[0] * 255 / 180;
		Mat S = channels[1];
		Mat V = channels[2];

		// asociere functie de tratare a evenimentelor MOUSE cu ferestra "src"
		// Ultimul parametru al functiei este adresa lamatricea H
		setMouseCallback("src", CallBackFuncL3, &V);
		imshow("src", srcg);

		// Wait until user press some key
		waitKey(0);
	}
}

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
		printf("16 - list hist \n\n");
		printf("21 - Lab 2 \n\n");
		printf("31 - Lab 3 - Region Growing Color (Hue) \n");
		printf("32 - Lab 3 - Region Growing Grayscale (Value) \n\n");
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
			case 31:
				L3_ColorModel_Build_Color();
				break;
			case 32:
				L3_ColorModel_Build_Grayscale();
				break;
		}
	}
	while (op!=0);
	return 0;
}
