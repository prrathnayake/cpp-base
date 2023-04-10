cmake_minimum_required(VERSION 3.25.2)

file(GLOB files
     "*/*.h"
     "*/*.cpp"
)

project(app_rabbitMQ)

find_package(rabbitmq-c REQUIRED)

add_library(${PROJECT_NAME} SHARED ${files})

target_link_libraries(${PROJECT_NAME}
     amqpcpp)
