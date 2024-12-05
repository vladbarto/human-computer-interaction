// //
// // Created by Vlad Bartolomei on 05.12.2024.
// //
// //
// // Created by Vlad Bartolomei on 01.12.2024.
// //
//
// #include "lab10.h"
//
// std::string image_path = "/Users/vladbarto/Documents/FACULTATE/AN4/SEM1/IOC/Lab_IOC/Lab_10_Detectia_de_persoane/Persons/person_133.bmp";
//
// void initClassifiersForPeopleDetection() {
//     fullBody_cascade.load("../classifiers/haar/haarcascade_fullbody.xml");
//     lowerBody_cascade.load("../classifiers/haar/haarcascade_lowerbody.xml");
//     upperBody_cascade.load("../classifiers/haar/haarcascade_upperbody.xml");
//     mcs_upperBody_cascade.load("../classifiers/haar/haarcascade_mcs_upperbody.xml");
// }
//
// void detectPeople() {
//     initClassifiersForPeopleDetection();
//
//     Mat src = imread(image_path, IMREAD_COLOR);
//     Mat gray = src.clone();
//     cvtColor(src, gray, COLOR_BGR2GRAY);
//     equalizeHist(gray, gray);
//
//     std::vector<Rect> bodies;
//
//     double t = (double)getTickCount();
//     fullBody_cascade.detectMultiScale(gray, bodies, 1.1, 5, 0, Size(0.3 * person_height, 0.3 * person_height));
//     if(bodies.empty()) {
//         std::cout<<" No full bodies detected"<< std::endl;
//     }
//
//     for(int i = 0; i < bodies.size(); i++) {
//         Point p1(
//             bodies[i].x,
//             bodies[i].y);
//         Point p2(
//             bodies[i].x + bodies[i].width,
//             bodies[i].y + bodies[i].height);
//         rectangle(src, p1, p2, CYAN, 2);
//     }
//
//     // Get the current time again and compute the time difference [s]
//     t = (static_cast<double>(getTickCount()) - t) / getTickFrequency();
//     // Print (in the console window) the processing time in [ms]
//     printf("Processing Time for FULLBODY - %.3f [ms]\n", t*1000);
//
// //------
//     std::vector<Rect> upperBodies;
//     t = (double)getTickCount();
//     upperBody_cascade.detectMultiScale(gray, upperBodies, 1.1, 5, 0, Size(0.3 * person_height, 0.5 * person_height));
//     if(upperBodies.empty()) {
//         std::cout<<" No upper bodies detected"<< std::endl;
//     }
//
//     for(int i = 0; i < upperBodies.size(); i++) {
//         Point p1(
//             upperBodies[i].x,
//             upperBodies[i].y);
//         Point p2(
//             upperBodies[i].x + upperBodies[i].width,
//             upperBodies[i].y + upperBodies[i].height);
//         rectangle(src, p1, p2, MAGENTA, 2);
//     }
//
//     // Get the current time again and compute the time difference [s]
//     t = (static_cast<double>(getTickCount()) - t) / getTickFrequency();
//     // Print (in the console window) the processing time in [ms]
//     printf("Processing Time for UPPERBODY - %.3f [ms]\n", t*1000);
//
//
// //------
//     std::vector<Rect> lowerBodies;
//     t = (double)getTickCount();
//     lowerBody_cascade.detectMultiScale(gray, lowerBodies, 1.1, 5, 0, Size(0.3 * person_height, 0.5 * person_height));
//     if(lowerBodies.empty()) {
//         std::cout<<" No lower bodies detected"<< std::endl;
//     }
//
//     for(int i = 0; i < lowerBodies.size(); i++) {
//         Point p1(
//             lowerBodies[i].x,
//             lowerBodies[i].y);
//         Point p2(
//             lowerBodies[i].x + lowerBodies[i].width,
//             lowerBodies[i].y + lowerBodies[i].height);
//         rectangle(src, p1, p2, YELLOW, 2);
//     }
//
//     // Get the current time again and compute the time difference [s]
//     t = (static_cast<double>(getTickCount()) - t) / getTickFrequency();
//     // Print (in the console window) the processing time in [ms]
//     printf("Processing Time for LOWERBODY - %.3f [ms]\n", t*1000);
//
//
//     imshow( "People detected", src ); //-- Show what you got
//     waitKey();
//
// }