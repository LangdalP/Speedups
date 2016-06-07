cmake_minimum_required(VERSION 2.8.9)
project (ImageProcessing)
 
set(SOURCES ./image_processor.cpp ./lodepng.cpp)

add_executable(image_processor ${SOURCES})