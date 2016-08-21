cmake_minimum_required(VERSION 2.8.9)
project (ImageProcessor)

if(PROFILE)
	set(CMAKE_BUILD_TYPE Release)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pg")
	message(STATUS "Building with profiling settings")
endif(PROFILE)

if(NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_CXX_FLAGS}")
endif()

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -march=native -mtune=native -O3 -std=c++14")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -std=c++14")
 
set(SOURCES ./image_processor.cpp ./lodepng.cpp ./lodepng.h)
add_executable(image_processor ${SOURCES})

configure_file(bloom_small.png bloom_small.png COPYONLY)

