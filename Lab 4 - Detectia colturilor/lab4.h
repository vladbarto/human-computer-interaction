//
// Created by Vlad Bartolomei on 30.10.2024.
//

#ifndef LAB4_H
#define LAB4_H
#include <vector>
#include "../common.h"
#include <fstream>

using namespace std;
using namespace cv;

static std::vector<Point2f> corners; // Lista/vector de iesire care va contine coordonatele (x,y) ale colturilor detectate (output)
static std::vector<Point2f> corners3;
static int maxCorners = 100; // Nr. maxim de colturi luate in considerare. Daca nr. de colturi > maxCorners se vor considera cele cu raspuns R maxim

// Factor cu care se multiplica masura de calitate a celui mai bun colt (val. proprie minima) pt. metoda Shi-Tomasi respectiv valoarea functiei de raspuns R (Harris)
//ex: qualityMeasure = 1500, qualityLevel = 0.01 => colturile cu valoarea mai mica de
//1500*0.01 sunt rejectate:
static double qualityLevel = 0.01;
static double minDistance = 10;// Raza vecinatatii pe care se aplica etapa de supresie a non-maximelor

// Dimensiunea ferestrei de integrare w folosita in calculul matriceei de autocorelatie M (covarianta a derivatelor)
static int blockSize = 3; // 2,3, ...
// Selectia metodei de detectie: Harris (true) sau Shi-Tomasi (false).
static bool useHarrisDetector = false;
// Factorul k (vezi descrierea teoretica a metodei)
static double k_4 = 0.04;

static Size winSize = Size( 5, 5 );
static Size zeroZone = Size( -1, -1 );
static TermCriteria criteria = TermCriteria( TermCriteria::EPS + TermCriteria::COUNT, 40, 0.001 );
static ofstream outFile("/Users/vladbarto/Documents/FACULTATE/AN4/SEM1/IOC/Lab_IOC/Lab4 - Detectia colturilor/FISIER_PCT_3.txt");

static Mat src, src_gray;
static int thresh = 200;
static int max_thresh = 255;
static const char* source_window = "Source image";
static const char* corners_window = "Corners detected";

void cornerDetectionWithSrc(Mat src_for_video);
void cornerDetectionDemo();
void cornerHarris_demo( int, void* );
void callCornerHarrisDemo();
void videoSequenceCornerDetection();

#endif //LAB4_H
