cmake_minimum_required(VERSION 3.25.2)
project(app_h3)

file(GLOB files
     "*/*.cpp"
)

find_package(h3 CONFIG REQUIRED)

add_library(${PROJECT_NAME} SHARED ${files})

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PUBLIC h3::h3)

install(
     DIRECTORY "${CMAKE_SOURCE_DIR}/h3"
     DESTINATION "include" 
     FILES_MATCHING 
     PATTERN "*.h" 
)

install(TARGETS ${PROJECT_NAME}
     RUNTIME DESTINATION bin
     ARCHIVE DESTINATION lib
     LIBRARY DESTINATION lib)
