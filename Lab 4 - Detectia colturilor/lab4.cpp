//
// Created by Vlad Bartolomei on 30.10.2024.
//
#include "lab4.h"

void cornerDetectionWithSrc(Mat src_for_video) {
    // Citestea imaginea din fisier

    Mat dst4_image = src_for_video.clone();

    Mat src4_image_gray;
    cvtColor(src_for_video, src4_image_gray, COLOR_BGR2GRAY);

    GaussianBlur(src4_image_gray, src4_image_gray, Size(5, 5), 0, 0);

    // Apel functie
    goodFeaturesToTrack( src4_image_gray,
        corners,
        maxCorners,
        qualityLevel,
        minDistance,
        Mat(), //masca pt. ROI - optional
        blockSize,
        useHarrisDetector,
        k_4 );

    for(auto punct : corners) {
        circle(dst4_image, (Point)punct, 5, Scalar(0, 255, 0), 2);
    }

    imshow("DEST1", dst4_image);
    //waitKey();
}

void cornerDetectionDemo() {
    // Citestea imaginea din fisier
    std::string fname;
    while (!(fname = openFileDlg()).empty()) {
        Mat src4_image = imread(fname.c_str(), IMREAD_COLOR) ;
        Mat dst4_image = src4_image.clone();

        Mat src4_image_gray;
        cvtColor(src4_image, src4_image_gray, COLOR_BGR2GRAY);

        GaussianBlur(src4_image_gray, src4_image_gray, Size(5, 5), 0, 0);

        // Apel functie
        goodFeaturesToTrack( src4_image_gray,
            corners,
            maxCorners,
            qualityLevel,
            minDistance,
            Mat(), //masca pt. ROI - optional
            blockSize,
            useHarrisDetector,
            k_4 );

        for(auto punct : corners) {
            circle(dst4_image, (Point)punct, 5, Scalar(0, 255, 0), 2);
        }

        imshow("DEST1", dst4_image);
        waitKey();

        // punctul 3 din indrumator
        // scrierea punctelor intr-un fisier text
        cornerSubPix(src4_image_gray, corners, winSize, zeroZone, criteria );

        std::cout<<"Corners sub pix:" << corners<<std::endl;

        for(auto punct : corners) {
            outFile << "[" << punct.x << ", " << punct.y << "]\n";
        }
        outFile.close();
    }
}

void cornerHarris_demo( int, void* )
{
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;
	Mat dst = Mat::zeros( src.size(), CV_32FC1 );
	cornerHarris( src_gray, dst, blockSize, apertureSize, k );
	Mat dst_norm, dst_norm_scaled;
	normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
	convertScaleAbs( dst_norm, dst_norm_scaled );

	int d = 5;
	//int w = 2*d + 1;

	for( int i = 0; i < dst_norm.rows - d; i++ )
	{
		for( int j = 0; j < dst_norm.cols - d; j++ )
		{
			bool flag = true;
			if( (int) dst_norm.at<float>(i,j) > thresh )
			{
				// parcurg vecinii din vecinatatea punctului curent (i, j)
				for(int ii = i - d; ii <= i + d; ii++) {
					for(int jj = j - d; jj <= j+d; jj++) {
						if( ii == i and jj == j) continue;
						if(dst_norm.at<float>(ii, jj) > dst_norm.at<float>(i, j)) flag = false;

					}
				}
				if(flag)
					circle( dst_norm_scaled, Point(j,i), 5, Scalar(0), 2, 8, 0 );
			}
		}
	}
	namedWindow( corners_window );
	imshow( corners_window, dst_norm_scaled );
}

void callCornerHarrisDemo() {
	// Citestea imaginea din fisier
	std::string fname;
	while (!(fname = openFileDlg()).empty()) {

		src = imread(fname.c_str(), IMREAD_COLOR);

		cvtColor( src, src_gray, COLOR_BGR2GRAY );
		namedWindow( source_window );
		createTrackbar( "Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo );
		imshow( source_window, src );
		cornerHarris_demo(0, 0);
		waitKey();
	}
}


void videoSequenceCornerDetection()
{
	//VideoCapture cap("/Users/vladbarto/Documents/FACULTATE/AN4/SEM1/IOC/Lab_IOC/Videos/taxi.avi"); // off-line video from file
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
		cornerDetectionWithSrc(frame);
		imshow("source", frame);

		c = waitKey(100);  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished\n");
			break;  //ESC pressed
		};
	}
}