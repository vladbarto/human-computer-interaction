//
// Created by Vlad Bartolomei on 30.10.2024.
//
#include "lab5.h"


void image_binarization() {
    int height = diff.cols;
    int width = diff.rows;
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++) {
            if(diff.at<uchar>(i, j) > Th) {
                dst.at<uchar>(i,j) = 255; // pentru toate metodele
            } else if(3 == method) {
                backgnd.at<uchar>(i, j) = alpha * gray.at<uchar>(i, j) + (1.0 - alpha) + backgnd.at<uchar>(i, j);
            }
        }
}

void lab5_background_substraction() {
    VideoCapture cap(
"/Users/vladbarto/Documents/FACULTATE/AN4/SEM1/IOC/Lab_IOC/Videos/laboratory.AVI"
);
    for(;;) {

        cap >> frame; // achizitie frame nou

        if(frame.empty()) {
            printf("End of video file\n");
            break;
        }

        ++frameNum;

        if (frameNum == 0)
            imshow( "sursa", frame); // daca este primul cadru se afiseaza doar sursa

        cvtColor(frame, gray, COLOR_BGR2GRAY);

        //Este recomandat sa aplicati un filtru trece jos (ex. Gaussian) pt. Eliminare zgomot
        GaussianBlur(gray, gray, Size(5, 5), 0.8, 0.8);

        //Se initializeaza matricea / imaginea destinatie pentru fiecare frame
        //dst = gray.clone();
        // sau
        // preferabil cu o imagine neagra
        dst = Mat::zeros( gray.size(), gray.type() );

        const int channels_gray = gray.channels();

        //restrictionam utilizarea metodei doar pt. imagini grayscale cu un canal (8 bit / pixel)
        if (channels_gray > 1) return;

        if (frameNum > 0) // daca nu este primul cadru
        {
            double t = (double)getTickCount(); // Get the current time [s]

            absdiff(gray, backgnd, diff);
            switch(method) {
                case 1:
                    backgnd = gray.clone();
                    break;
                case 2:
                    addWeighted(gray, alpha, backgnd, 1.0-alpha, 0, backgnd);
                    break;
                default:
                    break;
            }
            //------ SABLON DE PRELUCRARI PT. METODELE BACKGROUND SUBTRACTION -------
            // Calcul imagine diferenta dintre cadrul current (gray) si fundal (backgnd)
            // Rezultatul se pune in matricea/imaginea diff
            // Se actualizeaza matricea/imaginea model a fundalului (backgnd)
            // conform celor 3 metode:
            // met 1: backgnd = gray.clone();
            // met 2: addWeighted(gray, alpha, backgnd, 1.0-alpha, 0, backgnd);

            image_binarization();
            // Binarizarea matricii diferenta (pt. toate metodele):
            // Se parcurge sistematic matricea diff
            //daca valoarea pt. pixelul current diff.at<uchar>(i,j) > Th
            // marcheaza pixelul din imaginea destinatie ca obiect:

            // altfel
            // actualizeaza model background (doar pt. met 3), pt. fiecare pixel
            //-------------------------------------------------------------------------

            Mat element = getStructuringElement( MORPH_CROSS, Size( 3, 3 ) );
            erode ( dst, dst, element, Point(-1,-1), 2 );
            dilate( dst, dst, element, Point(-1,-1), 2 );

            // Get the current time again and compute the time difference [s]
            t = (static_cast<double>(getTickCount()) - t) / getTickFrequency();
            // Print (in the console window) the processing time in [ms]
            printf("%d - %.3f [ms]\n", frameNum, t*1000);

            namedWindow( "sursa" );
            createTrackbar( "Threshold: ", "sursa", &Th, 100, nullptr);
            // Afiseaza imaginea sursa si destinatie
            imshow( "sursa", frame); // show source
            imshow("diff", diff); // sa vedem si diferenta
            imshow("dest", dst); // show destination
        }
        else // daca este primul cadru, modelul de fundal este chiar el
            backgnd = gray.clone();
        // Conditia de avansare/terminare in buclei for(;;) de procesare
        c = waitKey(100); // press any key to advance between frames
        //for continous play use cvWaitKey( delay ), where delay > 0
        if (c == 27) {
            // press ESC to exit
            printf("ESC pressed - playback finished\n");
            break; //ESC pressed
        }
    }
}
