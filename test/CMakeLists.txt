cmake_minimum_required(VERSION 3.10)
project(popiniTest LANGUAGES C VERSION 0.1.0)

if(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release)
endif()

add_subdirectory("../" "lib/popini/")

add_executable(popiniTest "src/main.c")
target_link_libraries(popiniTest PRIVATE -static PRIVATE popini)
target_include_directories(popiniTest PRIVATE "../src")
set_target_properties(popiniTest PROPERTIES C_STANDARD 90 C_EXTENSIONS OFF)