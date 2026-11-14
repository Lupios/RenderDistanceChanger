cmake_minimum_required(VERSION 3.22)
project(RenderDistanceKeys LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)

set(PRELOADER_ANDROID_ROOT "" CACHE PATH "Pfad zu LiteLDev/preloader-android")
if(NOT EXISTS "${PRELOADER_ANDROID_ROOT}/include/pl/ModMenu.hpp")
  message(FATAL_ERROR "Preloader-Header nicht gefunden: ${PRELOADER_ANDROID_ROOT}")
endif()

if(NOT TARGET preloader)
  add_subdirectory("${PRELOADER_ANDROID_ROOT}"
                   "${CMAKE_BINARY_DIR}/_preloader_android")
endif()

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 11.2.0)
set(FMT_DOC OFF CACHE BOOL "" FORCE)
set(FMT_TEST OFF CACHE BOOL "" FORCE)
set(FMT_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(fmt)

find_library(LOG_LIB log)

add_library(RenderDistanceKeys SHARED src/main.cpp)
target_include_directories(RenderDistanceKeys PRIVATE
  "${PRELOADER_ANDROID_ROOT}/include")
target_compile_options(RenderDistanceKeys PRIVATE
  -Wall -Wextra -fvisibility=hidden -fvisibility-inlines-hidden)
target_link_libraries(RenderDistanceKeys PRIVATE fmt::fmt)
if(TARGET preloader)
  target_link_libraries(RenderDistanceKeys PRIVATE preloader)
else()
  target_link_options(RenderDistanceKeys PRIVATE
    -Wl,--unresolved-symbols=ignore-all)
endif()
if(LOG_LIB)
  target_link_libraries(RenderDistanceKeys PRIVATE ${LOG_LIB})
endif()
