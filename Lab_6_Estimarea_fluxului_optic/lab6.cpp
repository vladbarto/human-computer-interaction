//
// Created by Vlad Bartolomei on 07.11.2024.
//

#include "lab6.h"

void calcOpticalFlowHS(const Mat& prev, const Mat& crnt, float lambda, int n0, Mat& flow)
{
    Mat vx = Mat::zeros(crnt.size(), CV_32FC1); // matricea comp. x a fluxului optic
    Mat vy = Mat::zeros(crnt.size(), CV_32FC1); // matricea comp. y a fluxului optic
    Mat Et = Mat::zeros(crnt.size(), CV_32FC1); // derivatele temporale
    Mat Ex, Ey; // Matricele derivatelor spatiale (gradient)

    // Calcul componenta orizontala a gradientului
    Sobel(crnt, Ex, CV_32F, 1, 0);

    // Calcul componenta verticala a gradientului
    Sobel(crnt, Ey, CV_32F, 0, 1);

    // Calcul derivata temporala
    Mat prev_float, crnt_float; // matricile imaginii crnt sip rev se convertesc in float
    prev.convertTo(prev_float, CV_32FC1);
    crnt.convertTo(crnt_float, CV_32FC1);
    Et = crnt_float - prev_float;

    // Insercati codul aferent algoritmului Horn-Schunk
    //TODO: . . .****************

    int height = crnt.rows, width = crnt.cols;
    for(int idx = 1; idx <= n0; idx++) {
        for(int i = 1; i < height-1; i++) {
            for(int j = 1; j < width-1; j++) {
                // current pixel p
                float vxMediu = (
                    vx.at<float>(i-1, j) +
                    vx.at<float>(i+1, j) +
                    vx.at<float>(i, j-1) +
                    vx.at<float>(i, j+1)
                    ) /4.0f ;

                float vyMediu = (
                    vy.at<float>(i-1, j) +
                    vy.at<float>(i+1, j) +
                    vy.at<float>(i, j-1) +
                    vy.at<float>(i, j+1)
                    ) / 4.0f;

                float ALPHA = lambda * (
                    (Ex.at<float>(i, j) * vxMediu + Ey.at<float>(i, j) * vyMediu + Et.at<float>(i, j))
                    /
                    (1 + lambda * (Ex.at<float>(i, j)*Ex.at<float>(i, j) + Ey.at<float>(i, j)*Ey.at<float>(i, j)) )
                );

                vx.at<float>(i, j) = vxMediu - ALPHA * Ex.at<float>(i, j);
                vy.at<float>(i, j) = vyMediu - ALPHA * Ey.at<float>(i, j);

            }
        }
    }
    //END TODO: . . .****************


    // Compune comp. x si y ale fluxului optic intr-o matrice cu elemente de tip Point2f
    flow = convert2flow(vx, vy);

    // Vizualizare rezultate intermediare:
    // gradient,derivata temporala si componentele vectorilor de miscare sub forma unor
    // imagini grayscale obtinute din matricile de tip float prin normalizare
    Mat Ex_gray, Ey_gray, Et_gray, vx_gray, vy_gray;
    normalize(Ex, Ex_gray, 0, 255, NORM_MINMAX, CV_8UC1, Mat());
    normalize(Ey, Ey_gray, 0, 255, NORM_MINMAX, CV_8UC1, Mat());
    normalize(Et, Et_gray, 0, 255, NORM_MINMAX, CV_8UC1, Mat());
    normalize(vx, vx_gray, 0, 255, NORM_MINMAX, CV_8UC1, Mat());
    normalize(vy, vy_gray, 0, 255, NORM_MINMAX, CV_8UC1, Mat());
    imshow("Ex", Ex_gray);
    imshow("Ey", Ey_gray);
    imshow("Et", Et_gray);
    imshow("vx", vx_gray);
    imshow("vy", vy_gray);
}

void testOpticalFlow2()
{
    //-----
    // parameters for calcOpticalFlowPyrLK
    vector<Point2f> prev_pts; // vector of 2D points with previous image features
    vector<Point2f> crnt_pts;// vector of 2D points with current image (matched) features
    vector<uchar> status; // output status vector: 1 if the wlow for the corresponding feature was found. 0 otherwise
    vector<float> error; // output vector of errors; each element of the vector is set to an error for the corresponding feature
    Size winSize=Size(21,21); // size of the search window at each pyramid level - deafult (21,21)
    int maxLevel=3; // maximal pyramid level number - deafult 3
    //parameter, specifying the termination criteria of the iterative search algorithm
    // (after the specified maximum number of iterations criteria.maxCount or when the search window moves by less than criteria.epsilon
    // deafult 30, 0.01
    TermCriteria criteria=TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 20, 0.03);
    int flags=0;
    double minEigThreshold=1e-4;
    //-----

    Mat crnt; // current frame red as grayscale (crnt)
    Mat prev; // previous frame (grayscale)
    Mat flow; // flow - matrix containing the optical flow vectors/pixel
    char folderName[MAX_PATH];
    char fname[MAX_PATH];
    int frameNum = -1; //current frame counter

    if (openFolderDlg(folderName)==0)
        return;
    FileGetter fg(folderName,"bmp");
    // printf("gets here");

    while(fg.getNextAbsFile(fname))
    {
        crnt = imread(fname, IMREAD_GRAYSCALE);
        GaussianBlur(crnt, crnt, Size(5, 5), 0.8, 0.8);
        ++frameNum;
        if (frameNum > 0) // not the first frame
        {

            // Horn-Shunk
            double t = (double)getTickCount();
            // pentru metoda 2
            int maxCorners = 100;
            double qualityLevel = 0.01;
            double minDistance = 10.0;
            int blockSize = 3;
            bool useHarrisDetector = false;
            double k = 0.04;
            // everything lays inside this method:
            switch(OPTICAL_FLOW_METHOD) {
                case 1:
                    calcOpticalFlowHS(prev, crnt, LAMBDA, N_ZERO, flow);
                    break;
                case 2:
                    goodFeaturesToTrack(prev,prev_pts,
                        maxCorners,
                        qualityLevel,
                        minDistance,
                        Mat(),
                        blockSize,
                        useHarrisDetector,
                        k);
                    calcOpticalFlowPyrLK( prev, crnt, prev_pts, crnt_pts, status, error,
winSize, maxLevel, criteria );
                    break;
            }

            // Stop the proccesing time measure
            t = ((double)getTickCount() - t) / getTickFrequency();
            printf("%d - %.3f [ms]\n", frameNum, t * 1000);
            switch(OPTICAL_FLOW_METHOD) {
                case 1:
                    showFlow(std::string("WIN_DST"), prev, flow, 1, 4, true, true, false);
                    break;
                case 2:
                    showFlowSparse("DST", prev, prev_pts, crnt_pts, status, error, 2, true, true, true);
            }
        }

        // store crntent frame as previos for the next cycle
        prev = crnt.clone();

        imshow(fg.getFoundFileName(), crnt);
        if (waitKey()==27) //ESC pressed
            break;
    }
}