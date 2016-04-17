cmake_minimum_required(VERSION 3.5)
project(esp8266_distance_sensor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    docs/images/HC-SR04-lg.jpg
    docs/images/official-nodemcu-development-board.jpg
    lib/readme.txt
    src/main.cpp
    README.MD)

add_executable(esp8266_distance_sensor ${SOURCE_FILES})