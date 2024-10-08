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
        "Lab 1 - Spatii de culoare/lab1.cpp")

# Link OpenCV libraries and your new_target to main
target_link_libraries(main ${OpenCV_LIBS} new_target)
