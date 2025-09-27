cmake_minimum_required(VERSION 3.25.2)

project(app_config LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE files
     "${CMAKE_CURRENT_SOURCE_DIR}/config/*.cpp")

if(files STREQUAL "")
    message(WARNING "app_config: no source files were found")
endif()

find_package(app_utils REQUIRED CONFIG)

add_library(${PROJECT_NAME} SHARED ${files})
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        app_utils::app_utils
)

install(
     DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/config"
     DESTINATION "include"
     FILES_MATCHING
     PATTERN "*.h"
)

install(TARGETS ${PROJECT_NAME}
     EXPORT app_configTargets
     RUNTIME DESTINATION bin
     ARCHIVE DESTINATION lib
     LIBRARY DESTINATION lib)
