cmake_minimum_required(VERSION 3.25.2)
project(app_rabbitmq)

file(GLOB files
     "*/*.cpp"
)

find_package(rabbitmq-c REQUIRED)
find_library(AMQP_CPP_LIBRARY NAMES amqpcpp PATHS /usr/local/lib REQUIRED)

add_library(${PROJECT_NAME} SHARED ${files})
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PRIVATE ${AMQP_CPP_LIBRARY}
amqpcpp)


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
