//
// Created by Vlad Bartolomei on 24.10.2024.
//
#include "lab3.h"

//Functia CallBack care se apeleaza la declansarea evenimentelor de nmouse
void CallBackFuncL3(int event, int x, int y, int flags, void* userdata)
{
	float konst = 2.5;
	float T = konst * hue_std3;

	double hue_avg = 0.0 ;
    Mat* H = (Mat*)userdata;
    Rect roi; // regiunea de interes curenta (ROI)
    if (event == EVENT_LBUTTONDOWN) // se alege punctul de start
    {
	    // punctul de start al ROI
    	Pstart3.x = x;
    	Pstart3.y = y;
    	draw3 = true;
    	printf("Pstart3: (%d, %d) \n ", Pstart3.x, Pstart3.y);

    	int nr = 0;
    	for(int neigh_row = x - 1; neigh_row <= x + 1; neigh_row++) {
    		for(int neigh_col = y - 1; neigh_col <= y + 1; neigh_col++) {
    			if(neigh_row < 0 || neigh_row > (*H).rows || // verific ca coordonatele sa fie in dimensiunile lui H
					neigh_col < 0 || neigh_col > (*H).cols)
    				continue;
    			else {
    				hue_avg += (*H).at<uchar>(neigh_row, neigh_col);
    			}
    			nr++;
    		}
    	}
    	hue_avg /= (double)nr;

    	///////////////// REGION GROWING
    	Mat labels = Mat::zeros((*H).size(),CV_8UC1);
    	queue<Point> que;
    	int k3 = 255; //ethiceta curenta (direct alb)
    	int N3 = 1; // numarul de pixeli din regiune

    	que.push(Point(x, y)); // adauga element (seed point) in coada
    	// acesta primeste eticheta k

    	while (!que.empty()) {
    		// Retine poz. celui mai vechi element din coada
    		Point oldest = que.front();
    		que.pop(); // scoate element din coada
    		int xx = oldest.x; // coordonatele lui
    		int yy = oldest.y;

    		// Pentru fiecare vecin al pixelului (xx, yy) ale carui coordonate sunt in interiorul imaginii
    		for (int neigh_row = xx - 1; neigh_row <= xx + 1; neigh_row++) {
    			for (int neigh_col = yy - 1; neigh_col <= yy + 1; neigh_col++) {
    				if (neigh_row == x && neigh_col == y)
    					continue; // eu nu vreau sa imi ia si punctul curent

    				if (neigh_row < 0 || neigh_row > (*H).rows ||
						// verific ca coordonatele sa fie in dimensiunile lui H
						neigh_col < 0 || neigh_col > (*H).cols)
    					continue;
    				// else
    				uchar hue_vecin = (*H).at<uchar>(neigh_row, neigh_col);
    				uchar hue = (*H).at<uchar>(xx, yy);

    				// Daca abs(hue(vecin) – Hue_avg)<T si labels(vecin) == 0
    				if ((abs(hue_vecin) - hue_avg) < T
						&& labels.at<uchar>(neigh_row, neigh_col) == 0) {
    					// Aduga vecin la regiunea curenta
    					Point point2add = Point(neigh_row, neigh_col);
    					que.push(point2add);

    					// labels(vecin) = k
    					labels.at<uchar>(neigh_row, neigh_col) = k3;

    					// Actualizeaza Hue_avg (medie ponderata)
    					hue_avg = (N3 * hue_avg + hue) / (N3 + 1);

    					// Incrementeza N
    					N3++;
						}
    			}
    		}
    	}

    	Mat element1 = getStructuringElement(MORPH_CROSS, Size(3, 3));
    	// crearea unui element structural de dimensiune 3x3 de tip patrat (V8)
    	Mat element2 = getStructuringElement(MORPH_RECT, Size(3, 3));

    	// dilatare cu acest element structural (aplicata 2x)
    	dilate(labels, labels, element2, Point(-1, -1), 2);
    	//
    	// //eroziune cu acest element structural (aplicata 1x)
    	erode(labels, labels, element1, Point(-1, -1), 1);


    	imshow("Labels", labels);
    	waitKey(27);
    }
}

// Functia principala in care se citesc imaginile sursa si se face legatura intre
// functia de mouse CallBack si fereastra in caare este afisata imaginea sursa
void L3_ColorModel_Build_Color() {
	//Mat srcg; declarata global
	Mat hsv;
	// Citestea imaginea din fisier
    std::string fname;
    while (!(fname = openFileDlg()).empty())
    {
        src3 = imread(fname.c_str());
        int height = src3.rows;
        int width = src3.cols;
        // Aplicare FTJ gaussian pt. eliminare zgomote / netezire imagine
        // http://opencvexamples.blogspot.com/2013/10/applying-gaussian-filter.html
        GaussianBlur(src3, src3, Size(5, 5), 0, 0);

        //Creare fereastra pt. afisare
        namedWindow("src", 1);

        // Componenta de culoare Hue a modelului HSV
        Mat channels[3];
        cvtColor(src3, hsv, COLOR_BGR2HSV); // conversie RGB -> HSV
        split(hsv, channels);

        // Componentele de culoare ale modelului HSV
        Mat H = channels[0] * 255 / 180;
        Mat S = channels[1];
        Mat V = channels[2];

        // asociere functie de tratare a evenimentelor MOUSE cu ferestra "src"
        // Ultimul parametru al functiei este adresa lamatricea H
        setMouseCallback("src", CallBackFuncL3, &H);
        imshow("src", src3);

        // Wait until user press some key
        waitKey();
    }
}

void L3_ColorModel_Build_Grayscale()
{
	//Mat src3; declarata global
	Mat hsv;
	// Citestea imaginea din fisier
	std::string fname;
	while (!(fname = openFileDlg()).empty())
	{
		src3 = imread(fname.c_str());
		int height = src3.rows;
		int width = src3.cols;
		// Aplicare FTJ gaussian pt. eliminare zgomote / netezire imagine
		// http://opencvexamples.blogspot.com/2013/10/applying-gaussian-filter.html
		GaussianBlur(src3, src3, Size(5, 5), 0, 0);

		//Creare fereastra pt. afisare
		namedWindow("src", 1);

		// Componenta de culoare Hue a modelului HSV
		Mat channels[3];
		cvtColor(src3, hsv, COLOR_BGR2HSV); // conversie RGB -> HSV
		split(hsv, channels);

		// Componentele de culoare ale modelului HSV
		Mat H = channels[0] * 255 / 180;
		Mat S = channels[1];
		Mat V = channels[2];

		// asociere functie de tratare a evenimentelor MOUSE cu ferestra "src"
		// Ultimul parametru al functiei este adresa lamatricea H
		setMouseCallback("src", CallBackFuncL3, &V);
		imshow("src", src3);
		imshow("Value channel", V);

		// Wait until user press some key
		waitKey();
	}
}