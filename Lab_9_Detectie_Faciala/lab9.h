//
// Created by Vlad Bartolomei on 28.11.2024.
//

#ifndef LAB9_H
#define LAB9_H

#include "../common.h"
#include "../Lab_8_Faces/lab8.h"

static Scalar COLOR_RED = Scalar(0, 0, 255);
static Scalar COLOR_GREEN = Scalar(0, 255, 0);
static int frameNr = -1; //current frame counter
static Mat currFrame, currGray, currDst, currBackgnd, currDiff;
static char cmd;
static Rect faceROI;

void validareDetectie();

#endif //LAB9_H
