cmake_minimum_required(VERSION 3.17)

# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Set target name
project(Lab_IOC CXX)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Ensure OpenCV headers are available globally
include_directories(${OpenCV_INCLUDE_DIRS})

# Optional: print paths for debugging
message(STATUS "OpenCV include path: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")

# Add library (for common.cpp)
add_library(new_target common.cpp)

# Add executable (main.cpp)
add_executable(main main.cpp
        "Lab 1 - Spatii de culoare/lab1.h"
        "Lab 1 - Spatii de culoare/lab1.cpp"
        "Lab 2 - Clasificarea pixelilor pe baza modelului/lab2.cpp"
        "Lab 2 - Clasificarea pixelilor pe baza modelului/lab2.h"
        Functions.h
        Functions.cpp
        colorcode.h
        colorcode.cpp
        "Lab 3 - Segmentarea bazata pe regiuni/lab3.cpp"
        "Lab 3 - Segmentarea bazata pe regiuni/lab3.h"
        "Lab 4 - Detectia colturilor/lab4.cpp"
        "Lab 4 - Detectia colturilor/lab4.h"
        "Lab 5 - Segmentarea obiectelor in miscare prin background substraction/lab5.cpp"
        "Lab 5 - Segmentarea obiectelor in miscare prin background substraction/lab5.h"
        "Lab_6_Estimarea_fluxului_optic/lab6.cpp"
        "Lab_6_Estimarea_fluxului_optic/lab6.h"
        Lab_7_Estimarea_fluxului_optic_dens/lab7.cpp
        Lab_7_Estimarea_fluxului_optic_dens/lab7.h
        Lab_8_Faces/lab8.cpp
        Lab_8_Faces/lab8.h
        Lab_9_Detectie_Faciala/lab9.cpp
        Lab_9_Detectie_Faciala/lab9.h
        Lab_10_Detectia_de_persoane/lab10.cpp
        Lab_10_Detectia_de_persoane/lab10.h
        L10/dummy.cpp)

# Link OpenCV libraries and your new_target to main
target_link_libraries(main ${OpenCV_LIBS} new_target)
