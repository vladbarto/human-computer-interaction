//
// Created by Vlad Bartolomei on 08.10.2024.
//

#ifndef LAB1_H
#define LAB1_H

#include "../common.h"

void myBGR2HSV(const char* fname);
void testMyBGR2HSV();

void binarizareManualaPragUnic(int threshold, const char *fname);
void testBinarizareManualaPragUnic();

void binarizareGlobalaAutomata(const int hist[], const char* image_path);
void testBinarizareGlobalaAutomata();

void testMouseClick();
#endif //LAB1_H
