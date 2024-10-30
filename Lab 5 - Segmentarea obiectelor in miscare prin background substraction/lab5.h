//
// Created by Vlad Bartolomei on 30.10.2024.
//

#ifndef LAB5_H
#define LAB5_H
#include <iostream>
#include "../common.h"
using namespace std;
using namespace cv;

static Mat frame, gray; //current frame: original and gray
static Mat backgnd; // background model
static Mat diff; //difference image: |frame_gray - bacgnd|
static Mat dst; //output image/frame
static char c;
static int frameNum = -1; //current frame counter
static const int method = 1; // 1, 2 sau 3 (metode prevazute in lab)
// method =
// 1 - frame difference
// 2 - running average
// 3 - running average with selectivity
//static const unsigned char Th = 25; // de ajustat in functie de secventa
static int Th=25;
static const double alpha = 0.05;

void lab5_background_substraction();

#endif //LAB5_H
