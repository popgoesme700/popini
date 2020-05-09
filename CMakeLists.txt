cmake_minimum_required(VERSION 3.10)
project(popini LANGUAGES C VERSION 0.0.0.1)

if(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release)
endif()

SET(CMAKE_C_STANDARD 90)
SET(CMAKE_C_EXTENSIONS OFF)
SET(BUILD_SHARED_LIBS OFF)
SET(CMAKE_EXE_LINKER_FLAGS "-static")

add_library(popini "src/popini.c")
include_directories(${CMAKE_CURRENT_BINARY_DIR})