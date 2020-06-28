cmake_minimum_required(VERSION 3.10)
project(popini LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release)
endif()

SET(CMAKE_C_STANDARD 90)
SET(CMAKE_C_EXTENSIONS OFF)
SET(BUILD_SHARED_LIBS OFF)

if(NOT BUILD_SHARED_LIBS)
	add_library(popini STATIC "src/popini.c")
else()
	add_library(popini SHARED "src/popini.c")
endif()
target_include_directories(popini PRIVATE ${CMAKE_CURRENT_BINARY_DIR})