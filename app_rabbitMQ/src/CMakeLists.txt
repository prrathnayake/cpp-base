cmake_minimum_required(VERSION 3.25.2)

project(app_rabbitmq LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE files
     "${CMAKE_CURRENT_SOURCE_DIR}/rabbitMQ/*.cpp")

find_package(app_utils REQUIRED CONFIG)
find_package(rabbitmq-c QUIET)
find_library(AMQP_CPP_LIBRARY NAMES amqpcpp PATHS /usr/local/lib /usr/lib /usr/lib64 /usr/local/lib64)
find_path(AMQP_CPP_INCLUDE_DIR NAMES amqpcpp.h PATHS /usr/local/include /usr/include)

if(NOT rabbitmq-c_FOUND OR NOT AMQP_CPP_LIBRARY OR NOT AMQP_CPP_INCLUDE_DIR)
    message(WARNING "RabbitMQ dependencies were not found; building a header-only stub for app_rabbitmq")
    add_library(${PROJECT_NAME} INTERFACE)
    target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_definitions(${PROJECT_NAME} INTERFACE APP_RABBITMQ_MISSING_DEPENDENCIES)
    target_link_libraries(${PROJECT_NAME} INTERFACE app_utils::app_utils)
    return()
endif()

add_library(${PROJECT_NAME} SHARED ${files})
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${AMQP_CPP_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        rabbitmq::rabbitmq
        ${AMQP_CPP_LIBRARY}
        app_utils::app_utils
)

install(
     DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/rabbitMQ"
     DESTINATION "include"
     FILES_MATCHING
     PATTERN "*.h"
)

install(TARGETS ${PROJECT_NAME}
     EXPORT app_rabbitmqTargets
     RUNTIME DESTINATION bin
     ARCHIVE DESTINATION lib
     LIBRARY DESTINATION lib)
