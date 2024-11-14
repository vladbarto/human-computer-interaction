//
// Created by Vlad Bartolomei on 14.11.2024.
//

#ifndef LAB7_H
#define LAB7_H

#include "../common.h"
#include "../functions.h"

static int winSize_int;
static double pyr_scale = 0.5;
static int levels = 3; // recommended value
static int poly_n = 5;
static double poly_sigma = 1.1;
static int iterations = 10;
static int flags = OPTFLOW_FARNEBACK_GAUSSIAN; // sau 0
static float minVel = 0.8;

void farnebackOpticalFlowDemo();
#endif //LAB7_H
