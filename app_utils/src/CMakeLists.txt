cmake_minimum_required(VERSION 3.25.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(app_utils)

file(GLOB files
     "*/*/*.cpp"
)

find_package(nlohmann_json REQUIRED)

add_library(${PROJECT_NAME} SHARED ${files})
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)


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