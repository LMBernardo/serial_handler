cmake_minimum_required(VERSION 3.13)
project(serial_handler)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(RS-232)

add_executable(serial_handler
        RS-232/rs232.c
        RS-232/rs232.h
        serial_handler.cpp
        serial_handler.h)
