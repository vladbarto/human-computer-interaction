//
// Created by Vlad Bartolomei on 08.10.2024.
//

#include "lab1.h"

void myBGR2HSV(const char* fname)
{
    Mat rgb = imread(fname);
    Mat hsv;
    Mat channels[3];
    cvtColor(rgb, hsv, COLOR_BGR2HSV);
    split(hsv, channels);
    // Componentele de culoare ale modelului HSV
    Mat H = channels[0]*255/180;
    Mat S = channels[1];
    Mat V = channels[2];

    int histH[256] = {0};
    int histS[256] = {0};
    int histV[256] = {0};

    for(int i = 0; i < H.rows; i++) {
        for(int j = 0; j < H.cols; j++) {
            histH[H.at<uchar>(i, j)]++;
            histS[S.at<uchar>(i, j)]++;
            histV[V.at<uchar>(i, j)]++;
        }
    }
    imshow("input rgb image", rgb);
    imshow("input hsv image", hsv); // vizualizarea matricii hsv (ca un mat cu3 canale) nu are semnificatie vizuala utila / relevanta
    imshow("H", H);
    imshow("S", S);
    imshow("V", V);
    showHistogram("histograma H", histH, 255, 200);
    showHistogram("histograma S", histS, 255, 200);
    showHistogram("histograma V", histV, 255, 200);
    waitKey();
}

void testMyBGR2HSV() {

    std::string fname;
    while (!(fname = openFileDlg()).empty()) {
        myBGR2HSV(fname.c_str());
    }
}

//---

void binarizareManualaPragUnic(int threshold, const char *fname) {
    std::cout<<"[prag] "<<threshold<< std::endl;

    Mat rgb = imread(fname);
    Mat hsv;
    Mat channels[3];
    cvtColor(rgb, hsv, COLOR_BGR2HSV);
    split(hsv, channels);
    // Componentele de culoare ale modelului HSV
    Mat H = channels[0]*255/180;
    // Mat S = channels[1];
    // Mat V = channels[2];

    Mat H_binar = H.clone();
    for(int i = 0; i < H.rows; i++) {
        for(int j = 0; j < H.cols; j++) {
            if(H_binar.at<uchar>(i, j) < threshold) {
                H_binar.at<uchar>(i, j) = WHITE;
            }
            else {
                H_binar.at<uchar>(i, j) = BLACK;
            }
        }
    }
    imshow("Canal H binarizat", H_binar);
    waitKey();
}

void testBinarizareManualaPragUnic() {

    std::string fname;
    while (!(fname = openFileDlg()).empty()) {
        binarizareManualaPragUnic(30, fname.c_str());
    }
}

//---

void binarizareGlobalaAutomata(const int hist[], const char *image_path) {
    float epsilon = 0.3;
    int min = INT_MAX;
    int max = INT_MIN;
    int minIntensity = 0, maxIntensity = 0;
    for(int i = 0; i < 256; i++) // intensitatea minima si maxima din poza
    {
        if(hist[i] < min) {
            min = hist[i];
            minIntensity = i;
        }
        if(hist[i] > max) {
            max = hist[i];
            maxIntensity = i;
        }
    }

    int T = (minIntensity+maxIntensity)/2;
    int T_nou = T;
    do {
        T = T_nou;
        int miu_g1 = 0;
        int miu_g2 = 0;
        int N1 = 0;
        int N2 = 0;

        for(int g = 0; g < T; g++) {
            N1 += hist[g];
            miu_g1 += g * hist[g];
        }

        for(int g = T; g < 256; g++) {
            N2 += hist[g];
            miu_g2 += g * hist[g];
        }

        miu_g1 /= N1;
        miu_g2 /= N2;

        T_nou = (miu_g1 + miu_g2)/2;

    } while(abs(T_nou - T) < epsilon);

    binarizareManualaPragUnic(T_nou, image_path);
}

void testBinarizareGlobalaAutomata() {
    int hist[255] = {0};
    std::string fname;
    while (!(fname = openFileDlg()).empty()) {
        Mat src = imread(fname.c_str(), IMREAD_COLOR);
        computeHist(src, hist);
        binarizareGlobalaAutomata(hist, fname.c_str());
    }
}

//---

void testMouseClick()
{
    // Read image from file
    std::string fname;
    while (!(fname = openFileDlg()).empty())
    {
        Mat src = imread(fname);
        // global_rgb = src.clone();
        //Create a window
        namedWindow("My Window", 1);

        // TODO: pe src il convertesti cu HSV
        Mat hsv;
        cvtColor(src, hsv, COLOR_BGR2HSV);
        // TODO: "vezi ca src trebe declarat global... cumva"
        //set the callback function for any mouse event
        setMouseCallback("My Window", MyCallBackFuncHSV, &src);

        //show the image
        imshow("My Window", src);

        // Wait until user press some key
        waitKey(0);
    }
}
