cmake_minimum_required(VERSION 2.8.9)
project (ImageProcessing)

# Compile options for speed with auto-vectorization
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -march=native -mtune=native -O3")
# Compile options for speed with auto-vectorization + vectorizer output
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -march=native -mtune=native -O3 -fopt-info-vec-optimized")
# Compile options for speed WITHOUT auto-vectorization
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -march=native -mtune=native -O3 -fno-tree-vectorize -std=c++14")
 
set(SOURCES ./image_processor.cpp ./lodepng.cpp ./lodepng.h)
add_executable(image_processor ${SOURCES})
