//
// Created by Vlad Bartolomei on 12.12.2024.
//
#include "lab11.h"

#include "../Lab 1 - Spatii de culoare/lab1.h"

/* -----------------------------------------------------------------------------------
window_name - name of the detsination window in which the detection results are
displayed
frame - source image (1 channel grayscale or 3 channel color image
----------------------------------------------------------------------------------- */

Mat negativ(Mat src) {
    Mat dst = Mat::zeros(src.size(), src.type());
    imshow("src to neg", src);
    for(int i = 0; i < src.rows; i++) {
        for(int j = 0; j < src.cols; i++) {
            if(src.at<uchar>(i, j) == 0)
                dst.at<uchar>(i, j) = 255;
            else //implicit src (i, j) e 255
                dst.at<uchar>(i, j) = 0;
        }
    }

    return dst;
}

Mat binarizareManualaPragUnic3(int threshold, Mat src) {
    std::cout<<"[prag] "<<threshold<< std::endl;

    Mat gray = src.clone();

    Mat gray_binar = gray.clone();
    for(int i = 0; i < gray.rows; i++) {
        for(int j = 0; j < gray.cols; j++) {
            if(gray_binar.at<uchar>(i, j) < threshold) {
                gray_binar.at<uchar>(i, j) = WHITE;
            }
            else {
                gray_binar.at<uchar>(i, j) = BLACK;
            }
        }
    }

    return gray_binar;
}
const std::string image_path = "/Users/vladbarto/Documents/FACULTATE/AN4/SEM1/IOC/Lab_IOC/Lab_10_Detectia_de_persoane/Persons/person_138.bmp";

void BodyDetectandDisplayHOG(const string& window_name, Mat frame)
{
    Mat src2 = frame.clone();
    Mat src3 = src2.clone();
    Scalar color_green(0, 255, 0);
    Scalar color_magenta(255, 0, 255);
    Scalar color_yellow(0, 255, 255);
    Scalar color_cyan(255, 255, 0);
    Scalar color_blue(255, 0, 0);
    Scalar color_red(0, 0, 255);

    // Initializarea clasei HOGDescriptor
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

    // Detectia persoanelor în imagine
    vector<Rect> persons; // vector cu regiunile aferente persoanelor detectate
    vector<double> weights; // scor de confidenta (mai mare = detective mai buna)
    hog.detectMultiScale(frame, persons, weights);

    // Afisarea rezultatelor detectiei si a scorului de confidenta asociat
    for (int i = 0; i < persons.size(); i++)
    {
        rectangle(frame, persons[i], color_cyan, 1, 8, 0);

        char message[10];
        sprintf(message, "%.3f", weights[i]);
        putText(frame,
            message,
            Point(persons[i].x + 5, persons[i].y + 15),
        FONT_HERSHEY_SIMPLEX,
        0.5,
        color_red,
        2,
        8);
    }

    int *proc = (int*)malloc(persons.size() * sizeof(int));
    for(int i = 0; i < persons.size(); i++)
        proc[i] = 0;
    for(int i = 0; i < persons.size() - 1; i++) {
        for(int j = i+1; j < persons.size(); j++ ) {
            float areaRatio = float(RectArea(persons[i] & persons[j])
                /
                min_(RectArea (persons[i]), RectArea(persons[j])));

            if (abs(RectCenter(persons[i]).x - RectCenter(persons[j]).x) < min_(persons[i].width, persons[j].width)) {
                if(areaRatio > 0.85) {
                    ///Then pastrati detectia cu aria cea mai mare si eliminat detectia cu aria cea mai mica
                    if(RectArea(persons[i]) > RectArea(persons[j])) {
                        rectangle(src2, persons[i], color_green, 2, 8, 0);
                        char message[10];
                        sprintf(message, "%.3f", weights[i]);
                        putText(src2,message,Point(persons[i].x + 5, persons[i].y + 15),
                        FONT_HERSHEY_SIMPLEX,0.5,color_red,2,8);
                    } else {
                        rectangle(src2, persons[j], color_green, 2, 8, 0);
                        char message[10];
                        sprintf(message, "%.3f", weights[j]);
                        putText(src2,message,Point(persons[j].x + 5, persons[j].y + 15),
                        FONT_HERSHEY_SIMPLEX,0.5,color_red,2,8);
                    }
                    proc[i] = proc[j] = 1;
                }
            }

        }
    }
    for(int i = 0; i < persons.size(); i++) {
        if(proc[i] == 1)
            continue;
        rectangle(src2, persons[i], color_green, 2, 8, 0);
        char message[10];
        sprintf(message, "%.3f", weights[i]);
        putText(src2,message,Point(persons[i].x + 5, persons[i].y + 15),
        FONT_HERSHEY_SIMPLEX,0.5,color_red,2,8);
    }

    /// STEP 3
    for(int i = 0; i < persons.size(); i++) {
        // preprocesari
        Mat roi = frame(persons[i]);
        Mat gray;
        cvtColor(roi, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, gray, Size(5, 5), 0.8, 0.8);
        equalizeHist(gray, gray);

        // a. calcul binarizare automata
        int hist[256] = {0};
        computeHist(gray, hist);
        int Th = binarizareGlobalaAutomata2(hist);

        // b. binarizare + negativ
        Mat img_neg = binarizareManualaPragUnic3(Th, gray);
        imshow("img binarizata si negativa", img_neg);
        waitKey();
        // Mat img_neg = negativ(img_binara);

        // c. morfologie
        // Mat element = getStructuringElement( MORPH_CROSS, Size( 3, 3 ) );
        // erode ( img_neg, img_neg, element, Point(-1,-1), 2 );
        // dilate( img_neg, img_neg, element, Point(-1,-1), 2 );

        // d. proiectie orizontala
        int rows = img_neg.rows;
        int cols = img_neg.cols;
        int *h = (int*) malloc(sizeof(int) * rows);
        for(int i = 0; i < rows; i++) {
            h[i] = 0;
        }

        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                h[i] += (img_neg.at<uchar>(i, j) > 0)?1:0;
            }
            std::cout<<h[i]<<std::endl;
        }

        // e. Detectia punctului de variatie a proiectiei orizontale (ca vector)
        int k;
        for(k = rows - 1; k >= 0; k--) {
            if(h[k] > 5)
                break;
        }

        persons[i].height = k;
        rectangle(src3, persons[i], color_magenta, 2);

    }
    imshow("Src at step 1", frame);
    imshow("Src at step 2", src2);
    imshow("Src at step 3", src3);
    waitKey();
}

void testBodyDetectionHog() {
//138, 230
    Mat src = imread(image_path, IMREAD_COLOR);

    BodyDetectandDisplayHOG("Window name", src);
}