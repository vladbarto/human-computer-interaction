//
// Created by Vlad Bartolomei on 09.10.2024.
//
#include "lab2.h"

void L2_ColorModel_Init()
{
    memset(histc_hue, 0, sizeof(unsigned int) * MAX_HUE);
    printf("Initialize / reset the Global Hue histogram\n");
    waitKey(2000);
}

//Functia CallBack care se apeleaza la declansarea evenimentelor de nmouse
void CallBackFuncL2(int event, int x, int y, int flags, void* userdata)
{
    Mat* H = (Mat*)userdata;
    Rect roi; // regiunea de interes curenta (ROI)
    if (event == EVENT_LBUTTONDOWN)
    {
        // punctul de start al ROI
        Pstart.x = x;
        Pstart.y = y;
        draw = true;
        printf("Pstart: (%d, %d) ", Pstart.x, Pstart.y);
    }
    else if (event == EVENT_MOUSEMOVE)
    {
        if (draw == true) // actiune de mouse dragging in derulare (activ)
        {
            // desenarea se face intr-o copie a matricii sursa
            Mat temp = srcg.clone();
            rectangle(temp, Pstart, Point(x, y), Scalar(0, 255, 0), 1, 8, 0);
            imshow("src", temp);
        }
    }
    else if (event == EVENT_LBUTTONUP)
    {
        // punctul de final (diametral opus) al ROI rectangulare
        draw = false; // actiune de mouse dragging s-a terminat (inactiva)
        Pend.x = x;
        Pend.y = y;
        printf("Pend: (%d, %d) ", Pend.x, Pend.y);

        // sortare crescatoare a celor doua puncta selectate dupa x si y
        roi.x = min(Pstart.x, Pend.x);
        roi.y = min(Pstart.y, Pend.y);
        roi.width = abs(Pstart.x - Pend.x);
        roi.height = abs(Pstart.y - Pend.y);
        printf("Local ROI: (%d, %d), (%d, %d)\n", roi.x, roi.y,
        roi.x + roi.width, roi.y + roi.height);
        rectangle(srcg, roi, Scalar(0, 255, 0), 1, 8, 0);

        // desenarea selectiei rectangulare se face peste imaginea sursa
        imshow("src", srcg);
        int hist_hue[MAX_HUE]; // histograma locala a lui Hue
        memset(hist_hue, 0, MAX_HUE * sizeof(int));

        // Din toata imaginea H se selecteaza o subimagine (Hroi) aferenta ROI
        Mat Hroi = (*H)(roi);
        uchar hue;

        //construieste histograma locala aferente ROI
        for (int y = 0; y < roi.height; y++)
            for (int x = 0; x < roi.width; x++)
            {
                hue = Hroi.at<uchar>(y, x);
                hist_hue[hue]++;
            }
        int countg = 0, countl = 0;

        //acumuleaza histograma locala in cea globala
        for (int i = 0; i < MAX_HUE; i++) {
            histc_hue[i] += hist_hue[i];
            countg += histc_hue[i];
            countl += hist_hue[i];
        }
        printf("Histogram count (global / local) = %d / %d \n", countg, countl);
        // afiseaza histohrama locala
        showHistogram("H local histogram", hist_hue, MAX_HUE, 200);//, true);
        // afiseaza histohrama globala
        showHistogram("H global histogram", histc_hue, MAX_HUE, 200);//, true);
    }
}

// Functia principala in care se citesc imaginile sursa si se face legatura intre
// functia de mouse CallBack si fereastra in caare este afisata imaginea sursa
void L2_ColorModel_Build()
{
    //Mat srcg; declarata global
    Mat hsv;
    // Citestea imaginea din fisier
    std::string fname;
    while (!(fname = openFileDlg()).empty())
    {
        srcg = imread(fname.c_str());
        int height = srcg.rows;
        int width = srcg.cols;
        // Aplicare FTJ gaussian pt. eliminare zgomote / netezire imagine
        // http://opencvexamples.blogspot.com/2013/10/applying-gaussian-filter.html
        GaussianBlur(srcg, srcg, Size(5, 5), 0, 0);

        //Creare fereastra pt. afisare
        namedWindow("src", 1);

        // Componenta de culoare Hue a modelului HSV
        Mat channels[3];
        cvtColor(srcg, hsv, COLOR_BGR2HSV); // conversie RGB -> HSV
        split(hsv, channels);

        // Componentele de culoare ale modelului HSV
        Mat H = channels[0] * 255 / 180;
        Mat S = channels[1];
        Mat V = channels[2];

        // asociere functie de tratare a evenimentelor MOUSE cu ferestra "src"
        // Ultimul parametru al functiei este adresa lamatricea H
        setMouseCallback("src", CallBackFuncL2, &H);
        imshow("src", srcg);

        // Wait until user press some key
        waitKey(0);
    }
}

void L2_ColorModel_Save()
{
    int hue, sat, i, j;
    int histF_hue[MAX_HUE]; // histograma filtrata cu FTJ
    memset(histF_hue, 0, MAX_HUE*sizeof(unsigned int));
    //Filtrare histograma Hue (optional)
#define FILTER_HISTOGRAM 1
#if FILTER_HISTOGRAM == 1
    // filtrare histograma cu filtru gaussian 1D de dimensiune w=7
    float gauss[7];
    float sqrt2pi = sqrtf(2 * PI);
    float sigma = 1.5;
    float e = 2.718;
    float sum = 0;
    // Construire gaussian
    for (i = 0; i<7; i++) {
        gauss[i] = 1.0 / (sqrt2pi*sigma)* powf(e, -(float)(i - 3)*(i - 3)
        / (2 * sigma*sigma));
        sum += gauss[i];
    }
    // Filtrare cu gaussian
    for (j = 3; j<MAX_HUE - 3; j++)
    {
        for (i = 0; i<7; i++)
            histF_hue[j] += (float)histc_hue[j + i - 3] * gauss[i];
    }
#elif
    for (j = 0; j<MAX_HUE; j++)
        histF_hue[j] = histc_hue[j];
#endif // End of "Filtrare Gaussiana Histograma Hue"
    showHistogram("H global histogram", histc_hue, MAX_HUE, 200, true);
    showHistogram("H global filtered histogram", histF_hue, MAX_HUE, 200, true);

    // Wait until user press some key
    waitKey(0);

    //Parametrii modelului (histograma modelului, media si deviatia standard) se pot salva optional intr-un
    // fisier text(pentru verificare/depanare).
    // pregatire pt. scriere valoari model in fisier
    FILE *fp;
    // Hue
    fp = fopen("D:\\Hue.txt", "wt");
    fprintf(fp, "H=[\n");
    for (hue = 0; hue<MAX_HUE; hue++){
        fprintf(fp, "%d\n", histF_hue[hue]);
    }
    fprintf(fp, "];\n");
    fprintf(fp, "Hmean = %.0f ;\n", hue_mean);
    fprintf(fp, "Hstd = %.0f ;\n", hue_std);
    fclose(fp);
} //end of L2_ColorModel_Save()

void LabelingL2(const std::string& name, const Mat& src, bool output_format)
{
	Mat dst = Mat::zeros(src.size(), CV_8UC3);
	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;

	findContours(src, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	Moments m;
	if (contours.size() > 0)
	{
		int idx = 0;
		for (; idx >= 0; idx = hierarchy[idx][0])
		{
			const std::vector<Point>& c = contours[idx];
			m = moments(c);
			double arie = m.m00;

			if (arie > 100)
			{
				double xc = m.m10 / m.m00;
				double yc = m.m01 / m.m00;

				Scalar color(rand() & 255, rand() & 255, rand() & 255);
				if (output_format)
					drawContours(dst, contours, idx, color, FILLED, 8, hierarchy);
				else
					drawContours(dst, contours, idx, color, 1, 8, hierarchy);

				// Calculăm unghiul axei de alungire
				double mc20p = m.m20 / m.m00 - xc * xc;
				double mc02p = m.m02 / m.m00 - yc * yc;
				double mc11p = m.m11 / m.m00 - xc * yc;
				float teta = 0.5 * atan2(2 * mc11p, mc20p - mc02p);

				// Calculăm panta
				float slope = tan(teta);

				// Calculăm punctele de intersecție
				std::vector<Point> intercepts;

				// Puncte de intersecție cu marginile imaginii
				// 1. Intersecția cu x = 0
				float y_at_x0 = slope * (0 - xc) + yc;
				intercepts.push_back(Point(0, (int)y_at_x0));

				// 2. Intersecția cu x = width - 1
				float y_at_xw = slope * ((dst.cols - 1) - xc) + yc;
				intercepts.push_back(Point(dst.cols - 1, (int)y_at_xw));

				// 3. Intersecția cu y = 0
				float x_at_y0 = (0 - yc) / slope + xc;
				intercepts.push_back(Point((int)x_at_y0, 0));

				// 4. Intersecția cu y = height - 1
				float x_at_yh = ((dst.rows - 1) - yc) / slope + xc;
				intercepts.push_back(Point((int)x_at_yh, dst.rows - 1));

				// Filtrăm punctele valide
				Point valid_p1, valid_p2;
				int count = 0;

				for (const auto& p : intercepts)
				{
					if (p.x >= 0 && p.x < dst.cols && p.y >= 0 && p.y < dst.rows)
					{
						if (count == 0) valid_p1 = p;
						else valid_p2 = p;
						count++;
						if (count == 2) break; // Ne trebuie doar 2 puncte
					}
				}

				// Desenăm axa de alungire
				line(dst, valid_p1, valid_p2, Scalar(255, 0, 0), 2); // Linia roșie pentru axa de alungire

				// Desenăm centrul de masă
				DrawCross(dst, Point(xc, yc), 9, Scalar(255, 255, 255), 1);
			}
		}
	}

	imshow(name, dst);
	waitKey(0);
}


void Labeling_L2_manual(const string& name, const Mat& src, bool output_format)
{
	// dst - matrice RGB24 pt. afisarea rezultatului
	Mat dst = Mat::zeros(src.size(), CV_8UC3);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	// http://docs.opencv.org/2.4/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=findcontours#findcontours
	findContours(src, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	Moments m;
	if (contours.size() > 0)
	{
		// iterate through all the top-level contours,
		// draw each connected component with its own random color
		int idx = 0;
		for (; idx >= 0; idx = hierarchy[idx][0])
		{
			const vector<Point>& c = contours[idx];

			// http://docs.opencv.org/2.4/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=moments#moments
			m = moments(c); // calcul momente imagine
			double arie = m.m00; // aria componentei conexe idx

			if (arie > 100)
			{
				double xc = m.m10 / m.m00; // coordonata x a CM al componentei conexe idx
				double yc = m.m01 / m.m00; // coordonata y a CM al componentei conexe idx

				Scalar color(rand() & 255, rand() & 255, rand() & 255);

				// http://docs.opencv.org/2.4/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=drawcontours#drawcontours
				if (output_format) // desenare obiecte pline ~ etichetare
					drawContours(dst, contours, idx, color, FILLED, 8, hierarchy);
				else  //desenare contur obiecte
					drawContours(dst, contours, idx, color, 1, 8, hierarchy);

				Point center(xc, yc);
				int radius = 5;

				// afisarea unor cercuri in jurul centrelor de masa
				//circle(final, center, radius,Scalar(255,255,355), 1, 8, 0);

				// afisarea unor cruci peste centrele de masa
				DrawCross(dst, center, 9, Scalar(255, 255, 255), 1);

				// https://en.wikipedia.org/wiki/Image_moment
				//calcul axa de alungire folosind momentele centrate de ordin 2
				double mc20p = m.m20 / m.m00 - xc*xc; // double mc20p = m.mu20 / m.m00;
				double mc02p = m.m02 / m.m00 - yc*yc; // double mc02p = m.mu02 / m.m00;
				double mc11p = m.m11 / m.m00 - xc*yc; // double mc11p = m.mu11 / m.m00;
				float teta = 0.5*atan2(2 * mc11p, mc20p - mc02p);
				float teta_deg = teta * 180 / PI;

				printf("ID=%d, arie=%.0f, xc=%0.f, yc=%0.f, teta=%.0f\n", idx, arie, xc, yc, teta_deg);

				double slope = tan(teta);
				printf("[slope] %f\n", slope);

				// caz 1: x = 0
				double y0 = yc - slope * xc;

				// caz 2: x = w-1
				double yw = yc + slope * (src.cols - 1 - xc);

				// caz 3: y = 0
				double x0 = (xc * slope + yc)/slope;

				// caz 4: y = h-1
				double xh = (xc * slope - yc + (src.rows - 1))/slope;

				std::vector<Point> puncte;
				if(y0 > 0 && y0 < src.cols) {
					std::cout<<"[1] 0, "<<y0<<std::endl;
					puncte.push_back(Point(0, y0));
				}

				//if(yw > 0 && yw < src.cols) {
					std::cout<<"[2] w-1, "<<yw<<std::endl;
					puncte.push_back(Point(src.cols-1, yw));
				//}

				if(x0 > 0 && x0 < src.rows) {
					std::cout<<"[3] "<<x0 <<", 0"<<std::endl;
					puncte.push_back(Point(x0, 0));
				}

				if(xh > 0 && xh < src.rows) {
					std::cout<<"[4] "<<xh <<", h-1"<<std::endl;
					puncte.push_back(Point(xh, src.rows-1));
				}


				std::cout <<std::endl << puncte;
				// for(int i = 0; i < 2; i++)
				// 	line(dst, puncte.at(i), puncte.at(i+1), Scalar (255, 0, 0));
				line(dst, puncte.at(0), puncte.at(1), Scalar (0, 255, 0));
			}//Hand/03.bmp
		}
	}

	imshow(name, dst);
	waitKey(0);
}

void L2_work_Build()
{
    //Mat src_l2; declarata global
    Mat hsv;
    // Citestea imaginea din fisier
    std::string fname;
	while (!(fname = openFileDlg()).empty())
    {
        src_l2 = imread(fname);

        int height = src_l2.rows;
        int width = src_l2.cols;
        // Aplicare FTJ gaussian pt. eliminare zgomote / netezire imagine
        // http://opencvexamples.blogspot.com/2013/10/applying-gaussian-filter.html
        GaussianBlur(src_l2, src_l2, Size(5, 5), 0, 0);

        //Creare fereastra pt. afisare
        namedWindow("src_l2", 1);

        // Componenta de culoare Hue a modelului HSV
        Mat channels[3];
        cvtColor(src_l2, hsv, COLOR_BGR2HSV); // conversie RGB -> HSV
        split(hsv, channels);

        // Componentele de culoare ale modelului HSV
        Mat H = channels[0] * 255 / 180;
        Mat S = channels[1];
        Mat V = channels[2];

        //Pregatim o matrice destinatie, de dimensiunea H
        Mat dst = H.clone();

        float lower_bound = hue_mean - k*hue_std;
        float upper_bound = hue_mean + k*hue_std;
        if(lower_bound < 0) lower_bound = 0;
        if(upper_bound > MAX_HUE) upper_bound = MAX_HUE;

        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                uchar pixel_curent = H.at<uchar>(i, j);
                if(pixel_curent >= lower_bound && pixel_curent <= upper_bound) {
                    dst.at<uchar>(i, j) = WHITE; // obiect
                }
                else {
                    dst.at<uchar>(i, j) = BLACK; // fundal
                }
            }
        }

		// curatarea imaginii de particule imperfecte
		// crearea unui element structural de dimensiune 5x5 de tip cruce
		Mat element1 = getStructuringElement(MORPH_CROSS, Size(5, 5));
		//eroziune cu acest element structural (aplicata 1x)
		erode(dst, dst, element1, Point(-1, -1), 2);

		// crearea unui element structural de dimensiune 3x3 de tip patrat (V8)
		Mat element2 = getStructuringElement(MORPH_RECT, Size(3, 3));
		// dilatare cu acest element structural (aplicata 2x)
		dilate(dst, dst, element2, Point(-1, -1), 4);

		erode(dst, dst, element1, Point(-1, -1), 2);

		// asociere functie de tratare a evenimentelor MOUSE cu ferestra "src"
	     // Ultimul parametru al functiei este adresa lamatricea H
        //setMouseCallback("src", CallBackFuncL2, &H);
        imshow("src_l2", dst);

        // Wait until user press some key
        //waitKey(0);

    	Labeling_L2_manual("Mana segmentata pe obiecte", dst, false);
		//LabelingL2("xyz", dst, true);
    }
}