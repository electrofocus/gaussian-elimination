cmake_minimum_required(VERSION 3.17)
project(gaussian-elimination C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(gaussian-elimination main_lss_71_01.c lss_71_01.h lss_71_01.c)
