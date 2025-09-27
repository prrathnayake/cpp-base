cmake_minimum_required(VERSION 3.25.2)

project(app_cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(app_utils REQUIRED CONFIG)

file(GLOB_RECURSE files
     "${CMAKE_CURRENT_SOURCE_DIR}/cache/*.cpp")

if(files STREQUAL "")
    message(WARNING "app_cache: no source files were found")
endif()

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
     DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cache"
     DESTINATION "include"
     FILES_MATCHING
     PATTERN "*.h"
)

install(TARGETS ${PROJECT_NAME}
     EXPORT app_cacheTargets
     RUNTIME DESTINATION bin
     ARCHIVE DESTINATION lib
     LIBRARY DESTINATION lib)
