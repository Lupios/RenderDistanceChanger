cmake_minimum_required(VERSION 3.22)
project(RenderDistanceKeys CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LEVI_SDK_INCLUDE "" CACHE PATH "Include-Ordner des LeviLaunchroid SDK")

add_library(RenderDistanceKeys SHARED src/main.cpp)
if(LEVI_SDK_INCLUDE)
  target_include_directories(RenderDistanceKeys PRIVATE ${LEVI_SDK_INCLUDE})
endif() 
