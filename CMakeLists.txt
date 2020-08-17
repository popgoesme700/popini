cmake_minimum_required(VERSION 3.10)
project(popini LANGUAGES C VERSION 0.1.0)

if(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release)
endif()

OPTION(BUILD_STATIC "Build popini as a static library." ON)

if(BUILD_STATIC)
	add_library(popini STATIC "src/popini.c")
else()
	add_library(popini SHARED "src/popini.c")
endif()
target_include_directories(popini PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
set_target_properties(popini PROPERTIES C_STANDARD 90 C_EXTENSIONS OFF)