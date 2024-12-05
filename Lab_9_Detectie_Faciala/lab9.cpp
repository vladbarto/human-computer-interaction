//
// Created by Vlad Bartolomei on 28.11.2024.
//
#include "lab9.h"
int Th = 25;
float ky = 0.1, kx1 = 0.3, kx2 = 0.5;

void lab9_image_binarization() {
    int height = currDiff.cols;
    int width = currDiff.rows;
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++) {
            if(currDiff.at<uchar>(i, j) > Th) {
                currDst.at<uchar>(i, j) = 255; // pentru toate metodele
            }
        }
}

// Declararea structurii in care se vor tine datele aferente fiecarei etichete
// Puteti insera declaratiile de mai jos la inceputul functiei de procesare
typedef struct List {
    double arie;
    double xc;
    double yc;
} mylist;
Scalar color;
void validareDetectie() {
    VideoCapture cap(
// "/Users/vladbarto/Documents/FACULTATE/AN4/SEM1/IOC/Lab_IOC/Lab_9_Detectie_Faciala/test_msv1_short.avi");
0);

    CascadeClassifier face_cascade_1;
    face_cascade_1.load("../classifiers/haar/haarcascade_frontalface_alt.xml");

    for(;;) {
        color = COLOR_RED;
        cap >> currFrame; // achizitie frame nou

        if(currFrame.empty()) {
            printf("End of video file\n");
            break;
        }

        double t = (double)getTickCount();
        // step 1
        ++frameNr;

        cvtColor(currFrame, currGray, COLOR_BGR2GRAY);
        equalizeHist(currGray, currGray);
        std::vector<Rect> faces;
        face_cascade_1.detectMultiScale(currGray, faces, 1.1, 5, 0,Size(30, 30) );
        if (faces.empty()) {
            std::cout << "No faces detected" << std::endl;
            continue;
        }

        Rect faceROI = faces[0];

        Point p1(
            faces[0].x,
            faces[0].y);
        Point p2(
            faces[0].x + faces[0].width,
            faces[0].y + faces[0].height);

        // step 2
        currDst = Mat::zeros(currGray.size(), currGray.type());
        if(frameNr > 0) {
            absdiff(currGray, currBackgnd, currDiff);
            currBackgnd = currGray.clone();

            lab9_image_binarization();

            //step 3
            Mat element = getStructuringElement( MORPH_CROSS, Size( 3, 3 ) );
            erode ( currDst, currDst, element, Point(-1,-1), 1 );
            dilate( currDst, currDst, element, Point(-1,-1), 1 );

            //step 4
            Mat temp = currDst(faceROI);

            //steps 5 & 6
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;
            Mat roi = Mat::zeros(temp.rows, temp.cols, CV_8UC3);
            findContours(temp, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
            Moments m;
            vector<mylist> candidates;
            candidates.clear(); // se apeleaza pt. fiecare cadru

            if (contours.size() > 0)
            {
                // iterate through all the top-level contours,
                // draw each connected component with its own random color
                int idx = 0;
                for (; idx >= 0; idx = hierarchy[idx][0])
                {
                    const vector<Point>& c = contours[idx];
                    m = moments(c); // calcul momente
                    double arie = m.m00; // aria componentei conexe idx
                    double xc = m.m10 / m.m00; // coordonata x a CM al componentei conexe idx
                    double yc = m.m01 / m.m00; // coordonata y a CM al componentei conexe idx
                    Scalar color(rand() & 255, rand() & 255, rand() & 255);
                    drawContours(roi, contours, idx, color, FILLED, 8, hierarchy);

                    mylist elem;
                    elem.arie = arie;
                    elem.xc = xc;
                    elem.yc = yc;
                    candidates.push_back(elem);
                }

                List max1, max2;
                max1 = max2 = candidates.at(0);
                for(int i = 0; i < candidates.size(); i++) {
                    if(max1.arie < candidates.at(i).arie) {
                        max1 = candidates.at(i);
                    }
                }
                for(int i = 0; i < candidates.size(); i++) {
                    if(max2.arie < candidates.at(i).arie) {
                        if(candidates.at(i).arie < max1.arie)
                            max2 = candidates.at(i);
                    }
                }

                if(abs(max1.yc - max2.yc) < ky*roi.rows && max1.yc < roi.rows/2 && max2.yc < roi.rows/2) {
                    float dp = abs(max1.xc - max2.xc);
                    if(kx1*roi.cols < dp && dp < kx2*roi.cols) {
                        if(max1.xc > roi.cols/2 && max2.xc < roi.cols/2) {
                            DrawCross(roi, Point(max1.xc, max2.yc), 30, COLOR_RED, 2);
                            DrawCross(roi, Point(max2.xc, max2.yc), 30, COLOR_GREEN, 2);
                            color = COLOR_GREEN;
                        }
                    }
                }


            }


            // imshow("dest", currDst); // show destination
            imshow("roi", roi);

            //step 6

        }
        else
            currBackgnd = currGray.clone();

        t = (static_cast<double>(getTickCount()) - t) / getTickFrequency();
        // Print (in the console window) the processing time in [ms]
        printf("%d - %.3f [ms]\n", frameNr, t*1000);

        rectangle(currFrame, p1, p2, color, 2);
        imshow( "sursa", currFrame); // daca este primul cadru se afiseaza doar sursa
        // Conditia de avansare/terminare in buclei for(;;) de procesare
        cmd = waitKey(); // press any key to advance between frames
        //for continous play use cvWaitKey( delay ), where delay > 0
        if (cmd == 27) {
            // press ESC to exit
            printf("ESC pressed - playback finished\n");
            break; //ESC pressed
        }
    }

}
