cmake_minimum_required(VERSION 3.25.2)
project(app_utils)

file(GLOB files
     "*/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${files})
target_include_directories(${PROJECT_NAME} PUBLIC include)

install(
     DIRECTORY "${CMAKE_SOURCE_DIR}/utils" 
     DESTINATION "include" 
     FILES_MATCHING 
     PATTERN "*.h" 
)

install(TARGETS ${PROJECT_NAME}
     RUNTIME DESTINATION bin
     ARCHIVE DESTINATION lib
     LIBRARY DESTINATION lib)