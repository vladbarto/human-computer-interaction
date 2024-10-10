//
// Created by Vlad Bartolomei on 09.10.2024.
//

#ifndef LAB2_H
#define LAB2_H

#include "../common.h"
#include "../Functions.h"

#define MAX_HUE 256
// variabile globale
static int histc_hue[MAX_HUE]; // histograma cumulativa (in care se aduna valorile histogramelor locale)
static Point Pstart, Pend; // Punctele/colturile aferente selectiei ROI curente (declarate global)
static bool draw = false; // Variabila globala care arata starea actiune de mouse dragging: true - in derulare
static Mat srcg;
static float hue_mean = 17;
static float hue_std = 5;
static Mat src_l2;
static float k = 2.5;

// void L2_ColorModel_Init();
//void CallBackFuncL2(int event, int x, int y, int flags, void* userdata);
// void L2_ColorModel_Build();
// void L2_ColorModel_Save();
void Labeling_L2_manual(const string& name, const Mat& src, bool output_format);
void L2_work_Build();
#endif //LAB2_H
