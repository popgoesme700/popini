cmake_minimum_required(VERSION 3.10)
project(popiniTest LANGUAGES C VERSION 0.0.0.1)

if(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release)
endif()

SET(CMAKE_C_STANDARD 90)
SET(CMAKE_C_EXTENSIONS OFF)
SET(BUILD_SHARED_LIBS OFF)
SET(CMAKE_EXE_LINKER_FLAGS "-static")

add_subdirectory("../" "lib/popini/")

add_executable(popiniTest "src/main.c")
#include_directories("../src")
target_link_libraries(popiniTest popini)
target_include_directories(popiniTest PUBLIC "../src")