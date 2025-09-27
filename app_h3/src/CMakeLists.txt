cmake_minimum_required(VERSION 3.25.2)
project(app_h3)

file(GLOB files
     "${CMAKE_CURRENT_SOURCE_DIR}/h3/*.cpp"
)

if(NOT files)
    message(FATAL_ERROR "No source files found for ${PROJECT_NAME}")
endif()

add_library(${PROJECT_NAME} SHARED ${files})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

if(TARGET app_utils)
    target_link_libraries(${PROJECT_NAME} PUBLIC app_utils)
endif()

install(
     DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/h3"
     DESTINATION "include"
     FILES_MATCHING
     PATTERN "*.h"
)

install(TARGETS ${PROJECT_NAME}
     RUNTIME DESTINATION bin
     ARCHIVE DESTINATION lib
     LIBRARY DESTINATION lib)
