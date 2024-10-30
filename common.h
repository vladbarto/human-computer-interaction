//
// Created by Vlad Bartolomei on 07.10.2024.
//

#ifndef COMMON_H
#define COMMON_H

//#define MAIN_IMAGE_FOLDER_PATH "/Users/vladbarto/Documents/FACULTATE/AN4/SEM1/IOC/Lab_IOC/Images/Hand/"
#define MAIN_IMAGE_FOLDER_PATH "/Users/vladbarto/Documents/FACULTATE/AN4/SEM1/IOC/Lab_IOC/Lab 4 - Detectia colturilor/Img_corners/"
#define MAX_PATH 1024
#define BLACK 0
#define WHITE 255

#define PI 3.14159265
#define POS_INFINITY 1e30
#define NEG_INFINITY -1e30
#define max_(x,y) ((x) > (y) ? (x) : (y))
#define min_(x,y) ((x) < (y) ? (x) : (y))
#define isNan(x) ((x) != (x) ? 1 : 0)

#include <opencv2/opencv.hpp>
using namespace cv;

class common {

};

//extern Mat global_rgb;
void showHistogram(const std::string& name, int* hist, const int hist_cols, const int hist_height);
int openImage(const char *image_path);
std::string openFileDlg();
void computeHist(Mat src, int hist[]);
void MyCallBackFuncHSV(int event, int x, int y, int flags, void* param);

#endif //COMMON_H
