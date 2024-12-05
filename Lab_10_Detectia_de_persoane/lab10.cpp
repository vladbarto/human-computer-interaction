//
// Created by Vlad Bartolomei on 01.12.2024.
//

#include "lab10.h"
#include <stdlib.h>

std::string image_path = "/Users/vladbarto/Documents/FACULTATE/AN4/SEM1/IOC/Lab_IOC/Lab_10_Detectia_de_persoane/Persons/person_230.bmp";//238

void initClassifiersForPeopleDetection() {
    fullBody_cascade.load("../classifiers/haar/haarcascade_fullbody.xml");
    lowerBody_cascade.load("../classifiers/haar/haarcascade_lowerbody.xml");
    upperBody_cascade.load("../classifiers/haar/haarcascade_upperbody.xml");
    mcs_upperBody_cascade.load("../classifiers/haar/haarcascade_mcs_upperbody.xml");
}

void detectPeople() {
    initClassifiersForPeopleDetection();

    Mat src = imread(image_path, IMREAD_COLOR);
    Mat src2 = src.clone();
    Mat gray = src.clone();
    cvtColor(src, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);

    std::vector<Rect> fullBodies;
    std::vector<Rect> upperBodies;
    std::vector<Rect> lowerBodies;

    double t = (double)getTickCount();
    fullBody_cascade.detectMultiScale(gray, fullBodies, 1.1, 2, 0, Size(0.5f * minBodyHeight, minBodyHeight));
    upperBody_cascade.detectMultiScale(gray, upperBodies, 1.1, 2, 0, Size(0.5f * minBodyHeight, 0.5 * minBodyHeight));
    lowerBody_cascade.detectMultiScale(gray, lowerBodies, 1.1, 2, 0, Size(0.5f * minBodyHeight, 0.5 * minBodyHeight));

    if(fullBodies.empty()) {
        std::cout<<" No full bodies detected"<< std::endl;
    }

    if(upperBodies.empty()) {
        std::cout<<" No upper bodies detected"<< std::endl;
    }

    if(lowerBodies.empty()) {
        std::cout<<" No lower bodies detected"<< std::endl;
    }

    int hOffset = minBodyHeight * 0.5f; // de ajustat la nevoie
    int yOffset = minBodyHeight * 2.5f;
    int xAvg = 0; // media pe orizontala a centrelor
    int xStd = 0; // deviata standard pe orizontala a centrelor
    int luyDif = 0; // diferenta pe verticala a centrelor uperBody - lowerBody
    int fuyDif = 0; // diferenta pe verticala a centrelor uperBody - fullBody
    int lfyDif = 0; // diferenta pe verticala a centrelor fullBody - lowerBody
    //Vectori cu detectiile finale validate
    std::vector<Rect> persons;
    std::vector<float> personsCF;
    persons.clear();
    personsCF.clear();
    // Vectori (array 1D) in care marcam detectiile initiale procesate
    uchar* fproc = (uchar*)calloc(fullBodies.size(), sizeof(uchar));
    uchar* uproc = (uchar*)calloc(upperBodies.size(), sizeof(uchar));
    uchar* lproc = (uchar*)calloc(lowerBodies.size(), sizeof(uchar));

    for(int i = 0; i < fullBodies.size(); i++) {
        for(int j = 0; j < upperBodies.size(); j++) {
            for(int k = 0; k < upperBodies.size(); k++) {
                Point p1_f(
                    fullBodies[i].x,
                    fullBodies[i].y);
                Point p2_f(
                    fullBodies[i].x + fullBodies[i].width,
                    fullBodies[i].y + fullBodies[i].height);
                rectangle(src, p1_f, p2_f, CYAN, 2);

                Point p1_u(
                    upperBodies[j].x,
                    upperBodies[j].y);
                Point p2_u(
                    upperBodies[j].x + upperBodies[j].width,
                    upperBodies[j].y + upperBodies[j].height);
                rectangle(src, p1_u, p2_u, MAGENTA, 2);

                Point p1_l(
                    lowerBodies[k].x,
                    lowerBodies[k].y);
                Point p2_l(
                    lowerBodies[k].x + lowerBodies[k].width,
                    lowerBodies[k].y + lowerBodies[k].height);
                rectangle(src, p1_l, p2_l, YELLOW, 2);


                // criteriul a: cautam persoane la care sunt detectate toate cele 3 componentele
                if (fproc[i] == 0 && uproc[j] == 0 && lproc[k] == 0)
                {
                    xAvg = (RectCenter(fullBodies[i]).x + RectCenter(upperBodies[j]).x +
                    RectCenter(lowerBodies[k]).x)/3;
                    xStd = (
                        abs(RectCenter(fullBodies[i]).x-xAvg)
                        +
                        abs(RectCenter(upperBodies[j]).x-xAvg)
                        +
                        abs(RectCenter(lowerBodies[k]).x-xAvg)
                        )/3;
                    luyDif = RectCenter(lowerBodies[k]).y - RectCenter(upperBodies[j]).y;
                    fuyDif = RectCenter(fullBodies[i]).y - RectCenter(upperBodies[j]).y;
                    lfyDif = RectCenter(lowerBodies[k]).y - RectCenter(fullBodies[i]).y;
                    // exista aliniere pe orizontala + verifica offseturile verticale
                    if (xStd < hOffset && luyDif > 0 && fuyDif > 0 && lfyDif > 0 )
                    {
                        //verifica rapoartele de arie
                        float areaRatio = float(RectArea(fullBodies[i])) / RectArea (upperBodies[j] | lowerBodies[k] );
                        if (0.7 < areaRatio && areaRatio < 1.3)
                        {
                            persons.push_back(fullBodies[i] & (upperBodies[j] | lowerBodies[k]) );
                            personsCF.push_back(0.99);
                            fproc[i] = uproc[j] = lproc[k] = 1; // detectii procesate deja care se ignora la pasii urmatori
                        }
                    }
                }

                // criteriul b
                if(fproc[i] == 0 && uproc[j] == 0 && lproc[k] == 0)
                {
                    xAvg = (RectCenter(upperBodies[j]).x + RectCenter(lowerBodies[k]).x)/2;
                    xStd = (
                        abs(RectCenter(upperBodies[j]).x-xAvg)
                        +
                        abs(RectCenter(lowerBodies[k]).x-xAvg)
                        )/2;
                    luyDif = RectCenter(lowerBodies[k]).y - RectCenter(upperBodies[j]).y;

                    // exista aliniere pe orizontala + verifica offseturile verticale
                    if (xStd < hOffset && luyDif > 0 )
                    {
                        if(RectCenter(upperBodies[j]).y < RectCenter(lowerBodies[k]).y) // if upperBody ii deasupra lui lowerbody
                        {
                            float dist_u_l = abs(RectCenter(upperBodies[i]).y - RectCenter(lowerBodies[j]).y);
                            if(dist_u_l < yOffset) {
                                persons.push_back(upperBodies[j] | lowerBodies[k] );
                                personsCF.push_back(0.66);
                                fproc[i] = uproc[j] = lproc[k] = 1; // detectii procesate deja care se ignora la pasii urmatori
                            }
                        }
                    }
                }

                // criteriul c
                if(fproc[i] == 0 && uproc[j] == 0 && lproc[k] == 0) {
                    xAvg = (RectCenter(fullBodies[i]).x + RectCenter(upperBodies[j]).x)/2;
                    xStd = (
                        abs(RectCenter(fullBodies[i]).x-xAvg)
                        +
                        abs(RectCenter(upperBodies[j]).x-xAvg)
                        )/2;
                    fuyDif = RectCenter(fullBodies[i]).y - RectCenter(upperBodies[j]).y;

                    // exista aliniere pe orizontala + verifica offseturile verticale
                    if (xStd < hOffset && fuyDif > 0)
                    {
                        if(RectCenter(upperBodies[j]).y < RectCenter(fullBodies[i]).y)
                        {
                            // validam ariile
                            float areaRatio = float(RectArea(upperBodies[j] & fullBodies[i]) / RectArea(upperBodies[j]));
                            if(areaRatio > 0.5) {
                                persons.push_back(upperBodies[j] | fullBodies[i] );
                                personsCF.push_back(0.66);
                                fproc[i] = uproc[j] = lproc[k] = 1;
                            }
                        }
                    }
                }

                // criteriul d
                if(fproc[i] == 0 && uproc[j] == 0 && lproc[k] == 0) {
                    xAvg = (RectCenter(fullBodies[i]).x + RectCenter(lowerBodies[k]).x)/2;
                    xStd = (
                        abs(RectCenter(fullBodies[i]).x-xAvg)
                        +
                        abs(RectCenter(lowerBodies[k]).x-xAvg)
                        )/2;
                    lfyDif = RectCenter(lowerBodies[k]).y - RectCenter(fullBodies[i]).y;

                    // exista aliniere pe orizontala + verifica offseturile verticale
                    if (xStd < hOffset && lfyDif > 0)
                    {
                        if(RectCenter(lowerBodies[k]).y < RectCenter(fullBodies[i]).y)
                        {
                            // validam ariile
                            float areaRatio = float(RectArea(lowerBodies[k] & fullBodies[i]) / RectArea(lowerBodies[k]));
                            if(areaRatio > 0.5) {
                                persons.push_back(lowerBodies[k] | fullBodies[i] );
                                personsCF.push_back(0.66);
                                fproc[i] = uproc[j] = lproc[k] = 1;
                            }
                        }
                    }
                }


            }


        }


    }

    // criteriul e
    for(int i = 0; i < fullBodies.size(); i++) {
        if(fproc[i] == 1)
            continue;
        persons.push_back(fullBodies[i]);
        fproc[i] = 1;
        personsCF.push_back(0.33);
    }

    for(int i = 0; i < upperBodies.size(); i++) {
        if(uproc[i] == 1)
            continue;
        persons.push_back(upperBodies[i]);
        uproc[i] = 1;
        personsCF.push_back(0.33);
    }

    for(int i = 0; i < lowerBodies.size(); i++) {
        if(lproc[i] == 1)
            continue;
        persons.push_back(lowerBodies[i]);
        uproc[i] = 1;
        personsCF.push_back(0.33);
    }

    // Get the current time again and compute the time difference [s]
    t = (static_cast<double>(getTickCount()) - t) / getTickFrequency();
    // Print (in the console window) the processing time in [ms]
    printf("Processing Time for ALLBODY - %.3f [ms]\n", t*1000);

    for(int i = 0; i < persons.size(); i++) {
        Point p1(
            persons[i].x,
            persons[i].y);
        Point p2(
            persons[i].x + persons[i].width,
            persons[i].y + persons[i].height);
        rectangle(src2, p1, p2, GREEN, 3);

        char msg[100];
        sprintf(msg, "%f", personsCF[i]);
        putText(src2, msg, p1, FONT_HERSHEY_SIMPLEX, 0.7,  GREEN, 2);
        printf("%s\n", msg);
    }
    imshow( "People detected", src ); //-- Show what you got
    imshow("People validation", src2);
    waitKey();

}