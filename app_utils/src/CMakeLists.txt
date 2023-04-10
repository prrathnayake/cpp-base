cmake_minimum_required(VERSION 3.25.2)
project(app_utils)

file(GLOB files
     "*/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${files})
target_include_directories(${PROJECT_NAME} PUBLIC include)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "utils/index.h")
install(TARGETS ${PROJECT_NAME} DESTINATION "."
     PUBLIC_HEADER DESTINATION include
     RUNTIME DESTINATION bin
     ARCHIVE DESTINATION lib
     LIBRARY DESTINATION lib)
