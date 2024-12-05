//
// Created by Vlad Bartolomei on 01.12.2024.
//

#ifndef LAB10_H
#define LAB10_H

#include "../common.h"
#include "../Functions.h"

static CascadeClassifier fullBody_cascade;
static CascadeClassifier lowerBody_cascade;
static CascadeClassifier upperBody_cascade;
static CascadeClassifier mcs_upperBody_cascade;
static Scalar CYAN = Scalar(255, 255, 0);
static Scalar MAGENTA = Scalar(255, 0, 255);
static Scalar YELLOW = Scalar(0, 255, 255);
static Scalar GREEN = Scalar(0, 255, 0);
static int person_height = 150;
static int minBodyHeight = 150;

void detectPeople();


#endif //LAB10_H
