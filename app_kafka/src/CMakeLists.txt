cmake_minimum_required(VERSION 3.25.2)
project(app_kafka)

file(GLOB files
     "*/*.cpp"
)

find_package(RdKafka REQUIRED)

add_library(${PROJECT_NAME} SHARED ${files})
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} RdKafka::rdkafka++)

install(
     DIRECTORY "${CMAKE_SOURCE_DIR}/kafka"
     DESTINATION "include" 
     FILES_MATCHING 
     PATTERN "*.h" 
)

install(TARGETS ${PROJECT_NAME}
     RUNTIME DESTINATION bin
     ARCHIVE DESTINATION lib
     LIBRARY DESTINATION lib)
