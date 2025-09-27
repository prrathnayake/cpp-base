cmake_minimum_required(VERSION 3.25.2)

project(app_kafka LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE files
     "${CMAKE_CURRENT_SOURCE_DIR}/kafka/*.cpp")

find_package(RdKafka QUIET)

if(NOT RdKafka_FOUND)
    message(WARNING "RdKafka was not found; building a header-only stub for app_kafka")
    add_library(${PROJECT_NAME} INTERFACE)
    target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_definitions(${PROJECT_NAME} INTERFACE APP_KAFKA_MISSING_DEPENDENCIES)
    return()
endif()

add_library(${PROJECT_NAME} SHARED ${files})
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        RdKafka::rdkafka++
)

install(
     DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/kafka"
     DESTINATION "include"
     FILES_MATCHING
     PATTERN "*.h"
)

install(TARGETS ${PROJECT_NAME}
     EXPORT app_kafkaTargets
     RUNTIME DESTINATION bin
     ARCHIVE DESTINATION lib
     LIBRARY DESTINATION lib)
