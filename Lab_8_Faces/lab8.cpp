//
// Created by Vlad Bartolomei on 21.11.2024.
//
#include "lab8.h"

/* ------------------------------------------------------------------------------------
---
Detects all the faces and eyes in the input image
window_name - name of the destination window in which the detection results are
displayed
frame - source image
minFaceSize - minimum size of the ROI in which a Face is searched
minEyeSize - minimum size of the ROI in which an Eye is searched
acording to the antropomorphic features of a face, minEyeSize = minFaceSize / 5
Usage: FaceDetectandDisplay( “Dst”, dst, minFaceSize, minEyeSize );
---------------------------------------------------------------------------------------
- */

// String face_cascade = "/Users/vladbarto/Documents/FACULTATE/AN4/SEM1/IOC/Lab_IOC/Lab_8_Faces/Faces/Facesx12_small.bmp";

void init() {
    face_cascade.load("../classifiers/haar/haarcascade_frontalface_alt.xml");
    eyes_cascade.load("../classifiers/haar/haarcascade_eye_tree_eyeglasses.xml");
    nose_cascade.load("../classifiers/haar/haarcascade_mcs_nose.xml");
    mouth_cascade.load("../classifiers/haar/haarcascade_mcs_mouth.xml");
    lbp_cascade.load("../classifiers/lbp/lbpcascade_frontalface.xml");

    image_path = "/Users/vladbarto/Documents/FACULTATE/AN4/SEM1/IOC/Lab_IOC/Lab_8_Faces/Faces/Facesx12.bmp";
}

Rect faceDetectandDisplay_Images(const string& window_name, Mat frame, int minFaceSize, int minEyeSize, bool SIMPLIFIED) {
    std::vector<Rect> faces;
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 5, 0,Size(minFaceSize, minFaceSize) );

    for(int i = 0; i < faces.size(); i++)
    {
        double t = (double)getTickCount(); // Get the current time [s]
        // get the center of the face
        Point p1(
            faces[i].x,
            faces[i].y);
        Point p2(
            faces[i].x + faces[i].width,
            faces[i].y + faces[i].height);
        rectangle(frame, p1, p2, Scalar(255, 0, 255), 2);
        if(!SIMPLIFIED) {
            // draw circle around the face
            // ellipse( frame, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0,
            // 360, Scalar(255, 0, 255), 4, 8, 0 );

            Mat faceROI = frame_gray( faces[i] );
                // Rect eyes_rect;
                // eyes_rect.x = faces[i].x;
                // eyes_rect.y = faces[i].y + 0.2*faces[i].height;
                // eyes_rect.width = faces[i].width;
                // eyes_rect.height = 0.55*faces[i].height;
                // Mat faceROI = frame_gray( eyes_rect );
            std::vector<Rect> eyes;

            //-- In each face (rectangular ROI), detect the eyes
            eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0, Size(minEyeSize, minEyeSize) );

            for(int j = 0; j < eyes.size(); j++)
            {
                // get the center of the eye
                // atentie la modul in care se calculeaza pozitia absoluta a centrului ochiului
                // relativa la coltul stanga-sus al imaginii:
                Point center(
                    faces[i].x + eyes[j].x + eyes[j].width*0.5,
                    faces[i].y + eyes[j].y + eyes[j].height*0.5);

                int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );

                // draw circle around the eye
                circle( frame, center, radius, Scalar(255, 0, 0), 2, 8, 0 );
            }

            // NOSE
                // Rect nose_rect;
                // nose_rect.x = faces[i].x;
                // nose_rect.y = faces[i].y + 0.4*faces[i].height;
                // nose_rect.width = faces[i]. width;
                // nose_rect.height = 0.75*faces[i].height;
                // faceROI = frame_gray(nose_rect);
            std::vector<Rect> noses;
            //-- In each face (rectangular ROI), detect the nose
            nose_cascade.detectMultiScale(faceROI, noses, 1.1, 2, 0, Size(minNoseSize, minNoseSize));

            for(int j = 0; j < noses.size(); j++) {
                Point p1(
                    faces[i].x + noses[j].x,
                    faces[i].y + noses[j].y);
                Point p2(
                    faces[i].x + noses[j].x + noses[j].width,
                    faces[i].y + noses[j].y + noses[j].height);

                // draw rectangle around the nose
                rectangle(frame, p1, p2, Scalar(0, 255, 0));
            }

            // MOUTH
                // Rect mouth_rect;
                // mouth_rect.x = faces[i].x;
                // mouth_rect.y = faces[i].y + 0.7*faces[i].height;
                // mouth_rect.width = faces[i]. width;
                // mouth_rect.height = 0.99*faces[i].height;
                // faceROI = frame_gray(mouth_rect);
            std::vector<Rect> mouths;
            //-- In each face (rectangular ROI), detect the nose
            mouth_cascade.detectMultiScale(faceROI, mouths, 1.1, 2, 0, Size(minMouthSize, minMouthSize));

            for(int j = 0; j < mouths.size(); j++) {
                Point p1(
                    faces[i].x + mouths[j].x,
                    faces[i].y + mouths[j].y);
                Point p2(
                    faces[i].x + mouths[j].x + mouths[j].width,
                    faces[i].y + mouths[j].y + mouths[j].height);

                // draw rectangle around the nose
                rectangle(frame, p1, p2, Scalar(0, 0, 255));
            }

        }

        // Get the current time again and compute the time difference [s]
        t = (static_cast<double>(getTickCount()) - t) / getTickFrequency();
        // Print (in the console window) the processing time in [ms]
        printf("Time - %.3f [ms]\n", t*1000);
    }

    imshow( window_name, frame ); //-- Show what you got
    waitKey();

    return faces[0];
}

void testFaceDetectAndDisplay_Images() {
    init();
    Mat src = imread(image_path, IMREAD_COLOR);
    // imshow("src", src);
    // waitKey();
    faceDetectandDisplay_Images("Dst", src, minFaceSize, minEyeSize, false);
}

void testFaceDetectAndDisplay_Video_Haar() {
    init();
    //VideoCapture cap("./Videos/rubic.avi"); // off-line video from file
    VideoCapture cap(0);	// live video from web cam
    if (!cap.isOpened()) {
        printf("Cannot open video capture device.\n");
        waitKey();
        return;
    }

    Mat edges;
    Mat frame;
    char c;

    while (cap.read(frame))
    {
        Mat grayFrame;
        faceDetectandDisplay_Images("Dst", frame, minFaceSize, minEyeSize, true);
        c = waitKey();  // waits a key press to advance to the next frame
        if (c == 27) {
            // press ESC to exit
            printf("ESC pressed - capture finished\n");
            break;  //ESC pressed
        };
    }
}

void testFaceDetectAndDisplay_Video_LBP() {
    init();
    //VideoCapture cap("./Videos/rubic.avi"); // off-line video from file
    VideoCapture cap(0);	// live video from web cam
    if (!cap.isOpened()) {
        printf("Cannot open video capture device.\n");
        waitKey();
        return;
    }

    Mat edges;
    Mat frame;
    char c;

    while (cap.read(frame))
    {
        Mat grayFrame;
        faceDetectandDisplay_Images("Dst", frame, minFaceSize, minEyeSize, true);
        c = waitKey();  // waits a key press to advance to the next frame
        if (c == 27) {
            // press ESC to exit
            printf("ESC pressed - capture finished\n");
            break;  //ESC pressed
        };
    }
}

