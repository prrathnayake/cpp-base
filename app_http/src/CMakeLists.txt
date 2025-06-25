cmake_minimum_required(VERSION 3.25.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(app_http)

file(GLOB files
     "*/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${files})

target_include_directories(${PROJECT_NAME} PUBLIC include)

install(
    DIRECTORY "${CMAKE_SOURCE_DIR}/http" 
    DESTINATION "include" 
    FILES_MATCHING 
    PATTERN "*.h"
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)
