//
// Created by Vlad Bartolomei on 07.10.2024.
//

#include "common.h"
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

int openFolderDlg(char *folderName) {
    std::cout << "Enter the path of the folder you want to select: ";
    // std::cin.getline(folderName, MAX_PATH);
    scanf("%s", folderName);

    // Check if the entered path is a directory
    struct stat info;
    if (stat(folderName, &info) != 0) {
        std::cerr << "Error: Cannot access " << folderName << std::endl;
        return 0;
    } else if (!(info.st_mode & S_IFDIR)) {
        std::cerr << "Error: " << folderName << " is not a directory." << std::endl;
        return 0;
    }

    // Success
    return 1;
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
        imshow("rgb", temp);
    }
}

//---------------- Belonging to class FileGetter ------------------

#include <algorithm>
#include <cstring>
#include <dirent.h>
#include <string>
#include <vector>
#include <cctype>


FileGetter::FileGetter(const char* folderin, const char* extin)
    : currentIndex(0), hasFiles(false) {
    strncpy(folder, folderin, MAX_PATH - 1);
    folder[MAX_PATH - 1] = '\0';  // Ensure null termination

    // Set extension, default to "*" if none specified
    if (extin) {
        strncpy(ext, extin, sizeof(ext) - 1);
        ext[sizeof(ext) - 1] = '\0';
    } else {
        strcpy(ext, "*");
    }

    // Open directory and gather filenames
    DIR* dir = opendir(folder);
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_REG && matchesExtension(entry->d_name)) {
                files.emplace_back(entry->d_name);
            }
        }
        closedir(dir);

        // Sort filenames with natural ordering
        std::sort(files.begin(), files.end(), naturalCompare);
        hasFiles = !files.empty();
    }
}

FileGetter::~FileGetter() {}

// Helper function to check if a filename matches the specified extension
bool FileGetter::matchesExtension(const char* filename) {
    if (strcmp(ext, "*") == 0) return true;  // Match any extension
    const char* dot = strrchr(filename, '.');
    return (dot && strcmp(dot + 1, ext) == 0);
}

// Natural comparator for sorting filenames with numeric order
bool FileGetter::naturalCompare(const std::string& a, const std::string& b) {
    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        if (std::isdigit(a[i]) && std::isdigit(b[j])) {
            // Extract numbers and compare as integers
            size_t startA = i, startB = j;
            while (i < a.size() && std::isdigit(a[i])) i++;
            while (j < b.size() && std::isdigit(b[j])) j++;
            int numA = std::stoi(a.substr(startA, i - startA));
            int numB = std::stoi(b.substr(startB, j - startB));
            if (numA != numB) return numA < numB;
        } else {
            // Compare characters if not both digits
            if (a[i] != b[j]) return a[i] < b[j];
            i++;
            j++;
        }
    }
    return a.size() < b.size();
}

// Gets the next file name only (no path)
int FileGetter::getNextFile(char* fname) {
    if (!hasFiles || currentIndex >= files.size()) return 0;
    strncpy(fname, files[currentIndex].c_str(), MAX_PATH - 1);
    fname[MAX_PATH - 1] = '\0';
    currentIndex++;
    return 1;
}

// Gets the next file name with absolute path
int FileGetter::getNextAbsFile(char* fname) {
    if (!hasFiles || currentIndex >= files.size()) return 0;
    snprintf(fname, MAX_PATH, "%s/%s", folder, files[currentIndex].c_str());
    currentIndex++;
    return 1;
}

// Returns the current file name found, without path
const char* FileGetter::getFoundFileName() {
    if (!hasFiles || currentIndex >= files.size()) return nullptr;
    return files[currentIndex - 1].c_str();
}
