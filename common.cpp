//
// Created by Vlad Bartolomei on 07.10.2024.
//

#include "common.h"
#include <iostream>
using namespace std;

void showHistogram(const string& name, int* hist, const int hist_cols, const int hist_height) {

    Mat imgHist(hist_height, hist_cols, CV_8UC3, CV_RGB(255, 255, 255));
    // constructs a white image

    //computes histogram maximum
    int max_hist = 0;
    for (int i = 0; i < hist_cols; i++)
        if (hist[i] > max_hist)
            max_hist = hist[i];

    double scale = 1.0;
    scale = (double)hist_height / max_hist;
    int baseline = hist_height - 1;
    for (int x = 0; x < hist_cols; x++) {
        Point p1 = Point(x, baseline);
        Point p2 = Point(x, baseline - cvRound(hist[x] * scale));
        line(imgHist, p1, p2, CV_RGB(255, 0, 255)); // histogram bins
        // colored in magenta

    }
    imshow(name, imgHist);
}

/**
 * Code snippet taken from https://docs.opencv.org/4.x/db/deb/tutorial_display_image.html
 * @param image_path
 * @return success code 0, or failure code 1
 */
int openImage(const char *image_path) {

    Mat img = imread(image_path, IMREAD_COLOR);

    if(img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }

    imshow("Display window", img);
    waitKey();
    return 0;
}

/**
 * Given: the path of the directory with Images
 * MUSAI SA BAGI LA TASTATURA CALEA RELATIVA DE LA DIRECTOR PANA LA IMAGINE!!!
 * @return the full path of an image, with extension included
 */
std::string openFileDlg() {
    system("clear");
    std::string temp = std::string(MAIN_IMAGE_FOLDER_PATH);
    char image_path[MAX_PATH];

    std::cout << "The main path of the folder with the images is already saved in memory." << std::endl;
    std::cout << "Expecting the relative path of the file: " << std::endl;
    std::cout << "(note: for exit type 'exit')" << std::endl;
    std::cout << MAIN_IMAGE_FOLDER_PATH << " % ";
    scanf("%s", &image_path);
    std::cout << endl;

    if(!(strstr(image_path, ".bmp")
        || strstr(image_path, ".png")
        || strstr(image_path, ".jpg"))
        )
    {
        return {""};
    }
    else {
        temp.append(image_path);
        return temp;
    }
}

void computeHist(Mat src, int hist[]) {
    for(int i = 0; i < src.rows; i++) {
        for(int j = 0; j < src.cols; j++) {
            hist[src.at<uchar>(i, j)]++;
        }
    }
}

void MyCallBackFuncHSV(int event, int x, int y, int flags, void* param)
{
    //More examples: http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
    Mat* rgb = (Mat*)param;
    Mat temp = (*rgb).clone();
    Mat hsv;
    cvtColor(*rgb, hsv, COLOR_BGR2HSV);

    if (event == EVENT_LBUTTONDOWN)
    {
        // Afisarea in consola
        printf("Pos(x,y): %d,%d  Color(H,S,V): %d,%d,%d\n",
            x, y,
            //TODO: normalize - DONE
            (int)(hsv).at<Vec3b>(y, x)[0] * 255/180,
            (int)(hsv).at<Vec3b>(y, x)[1],
            (int)(hsv).at<Vec3b>(y, x)[2]);

        // Afisarea in imaginea originala
        char msg[100];
        sprintf(msg, "Pos(x,y): %d,%d  Color(H,S,V): %d,%d,%d\n",
            x, y,
            (int)(hsv).at<Vec3b>(y, x)[0] * 255/180,
            (int)(hsv).at<Vec3b>(y, x)[1],
            (int)(hsv).at<Vec3b>(y, x)[2]);
        putText(temp, msg, Point(5, 20), FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0, 255, 0), 1, 8);
        std::cout<<msg<<"\n";
        imshow("rgb", temp);
    }
}