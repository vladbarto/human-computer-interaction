//
// Created by Vlad Bartolomei on 14.11.2024.
//
#include "lab7.h"

int det_max_on_hist(int hist[]) {
    int bin_max = 0;
    int maxim = hist[0];
    for(int i = 1; i < 360; i++) {
        if(hist[i] > maxim) {
            bin_max = i;
            maxim = hist[i];
        }
    }

    return bin_max;
}
void farnebackOpticalFlowDemo() {
    Mat crnt; // current frame red as grayscale (crnt)
    Mat prev; // previous frame (grayscale)
    Mat flow; // flow - matrix containing the optical flow vectors/pixel
    char folderName[MAX_PATH];
    char fname[MAX_PATH];
    int frameNum = -1; //current frame counter

    if (openFolderDlg(folderName)==0)
        return;
    FileGetter fg(folderName,"bmp");

    while(fg.getNextAbsFile(fname))
    {
        crnt = imread(fname, IMREAD_GRAYSCALE);
        GaussianBlur(crnt, crnt, Size(5, 5), 0.8, 0.8);
        ++frameNum;
        int hist_dir[360]={0};
        if (frameNum > 0) // not the first frame
        {
            double t = (double)getTickCount();

            for (int r = 0; r < flow.rows; r++) {
                for(int c = 0; c < flow.cols; c++) {
                    Point2f f = flow.at<Point2f>(r, c); // vectorul de miscare in punctual (r,c)
                    float dir_rad = CV_PI + atan2(-f.y, -f.x); //directia vectorului in radiani
                    int dir_deg = (int)(dir_rad*180/CV_PI) % 360 ;

                    float magnitude = sqrt(f.x*f.x + f.y*f.y);
                    if(magnitude >= minVel) {
                        hist_dir[dir_deg]++;
                    }
                }

            }

            // vectorul de miscare al punctului se considera cu originea in imaginea trecuta (prev)
            // si varful in imaginea curenta (crnt) –> se iau valorile lui din vectorul flow cu minus !
           //folositi aceasta valuare la construirea histogramei directiilor vectorilor de miscare
            winSize_int = 11;
            calcOpticalFlowFarneback(prev, crnt, flow, pyr_scale, levels, winSize_int, iterations, poly_n, poly_sigma, flags);

            showFlowDense("Harta densa a fluxului optic", crnt, flow, minVel, true);
            showHistogram ("Hist", hist_dir, 360, 200, true);
            showHistogramDir("HistDir", hist_dir, 360, 200, true);
            // 200 [pixeli] = inaltimea ferestrei de afisare a histogramei



            // Stop the proccesing time measure
            t = ((double)getTickCount() - t) / getTickFrequency();
            printf("%d - %.3f [ms]\n", frameNum, t * 1000);
        }

        // store crntent frame as previos for the next cycle
        prev = crnt.clone();
        char buffer[50];
        sprintf(buffer, "Hist_max = %d", det_max_on_hist(hist_dir));
        putText(crnt, buffer, Point(30 , 50), FONT_HERSHEY_SCRIPT_COMPLEX, 1, Scalar (0, 255, 255), 1, LINE_AA);
        imshow(fg.getFoundFileName(), crnt);

        if (waitKey()==27) //ESC pressed
            break;
    }
}
