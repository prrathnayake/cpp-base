cmake_minimum_required(VERSION 3.25.2)

project(cppBase VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(CPPBASE_BUILD_UTILS "Build the shared utility library" ON)
option(CPPBASE_BUILD_KAFKA "Build the Kafka integration library" ON)
option(CPPBASE_BUILD_RABBITMQ "Build the RabbitMQ integration library" ON)

set(_cppbase_enabled_targets)

if(CPPBASE_BUILD_UTILS)
    add_subdirectory(app_utils/src)
    if(TARGET app_utils)
        list(APPEND _cppbase_enabled_targets app_utils)
    endif()
endif()

if(CPPBASE_BUILD_KAFKA)
    add_subdirectory(app_kafka/src)
    if(TARGET app_kafka)
        list(APPEND _cppbase_enabled_targets app_kafka)
    endif()
endif()

if(CPPBASE_BUILD_RABBITMQ)
    add_subdirectory(app_rabbitMQ/src)
    if(TARGET app_rabbitmq)
        list(APPEND _cppbase_enabled_targets app_rabbitmq)
    endif()
endif()

if(_cppbase_enabled_targets)
    add_library(cppbase INTERFACE)
    target_link_libraries(cppbase INTERFACE ${_cppbase_enabled_targets})
endif()