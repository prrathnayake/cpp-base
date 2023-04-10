cmake_minimum_required(VERSION 3.25.2)
project(app_rabbitMQ)

file(GLOB files
     "*/*.cpp"
)

find_package(rabbitmq-c REQUIRED)

add_library(${PROJECT_NAME} SHARED ${files})
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} amqpcpp)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "rabbitMQ/index.h")
install(TARGETS ${PROJECT_NAME} DESTINATION "."
     PUBLIC_HEADER DESTINATION include
     RUNTIME DESTINATION bin
     ARCHIVE DESTINATION lib
     LIBRARY DESTINATION lib)
