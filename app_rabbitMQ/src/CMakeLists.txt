cmake_minimum_required(VERSION 3.25.2)
project(app_rabbitmq)

file(GLOB files
     "*/*.cpp"
)

find_package(rabbitmq-c REQUIRED)

add_library(${PROJECT_NAME} SHARED ${files})
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} amqpcpp)

install(
     DIRECTORY "${CMAKE_SOURCE_DIR}/rabbitMQ"
     DESTINATION "include" 
     FILES_MATCHING 
     PATTERN "*.h" 
)

install(TARGETS ${PROJECT_NAME}
     RUNTIME DESTINATION bin
     ARCHIVE DESTINATION lib
     LIBRARY DESTINATION lib)
