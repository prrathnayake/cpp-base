cmake_minimum_required(VERSION 3.25.2)

file(GLOB files
     "*/*.h"
     "*/*.cpp"
)

project(app_kafka)

find_package(RdKafka REQUIRED)

add_library(${PROJECT_NAME} SHARED ${files})

target_link_libraries(${PROJECT_NAME}
    RdKafka::rdkafka++)