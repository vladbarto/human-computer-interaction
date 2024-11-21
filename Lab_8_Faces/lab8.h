//
// Created by Vlad Bartolomei on 21.11.2024.
//

#ifndef LAB8_H
#define LAB8_H

#include "../common.h"
#include "../Functions.h"

static String image_path;
static CascadeClassifier face_cascade;
static CascadeClassifier eyes_cascade;
static CascadeClassifier nose_cascade;
static CascadeClassifier mouth_cascade;
static CascadeClassifier lbp_cascade;
static int minFaceSize = 30; //sigur
static int maxFaceSize = 60;
static int minEyeSize = minFaceSize /5; // conform proprietatilor antropomorfice ale fetei (idem pt. gura si nas)
static int minNoseSize = minFaceSize / 5;
static int minMouthSize = minFaceSize / 4;

void testFaceDetectAndDisplay_Images();
void testFaceDetectAndDisplay_Video();
void testFaceDetectAndDisplay_Video_LBP();

#endif //LAB8_H
