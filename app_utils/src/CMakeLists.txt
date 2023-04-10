cmake_minimum_required(VERSION 3.25.2)
project(app_utils)

file(GLOB files
     "*/*.h"
     "*/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${files})
