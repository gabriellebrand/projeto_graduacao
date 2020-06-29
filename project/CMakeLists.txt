cmake_minimum_required(VERSION 3.15)
project(project)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "/usr/local/opt/opencv@3/share/OpenCV")

set(OpenCV_INCLUDE_DIRS "/usr/local/opt/opencv@3/include")
set(OpenCV_LIBS "/usr/local/opt/opencv@3/lib")

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

# Add OpenCV headers location to your include paths
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(project main.cpp path_queue.h
        path_queue.cpp path_support.h path_support.c
        PathOpen.cpp PathOpen.h Operators/FractureOperator.cpp Operators/FractureOperator.h
        Operators/BaseOperator.cpp Operators/BaseOperator.h Utils.h dump.h Operators/VerticalOperator.cpp
        Operators/VerticalOperator.h Operators/HorizontalOperator.cpp Operators/HorizontalOperator.h
        Operators/DiagonalLeftOperator.cpp Operators/DiagonalLeftOperator.h Operators/DiagonalRightOperator.cpp Operators/DiagonalRightOperator.h Adjacency.h Operators/FractureRightOperator.cpp Operators/FractureRightOperator.h)

# Link your application with OpenCV libraries
target_link_libraries(project ${OpenCV_LIBS})