cmake_minimum_required(VERSION 3.25.2)
project(app_blockchain)

file(GLOB files
     "*/*.cpp"
)

find_package(OpenSSL REQUIRED)

add_library(${PROJECT_NAME} SHARED ${files})
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries( ${PROJECT_NAME}
     OpenSSL::SSL
     OpenSSL::Crypto
)

install(
     DIRECTORY "${CMAKE_SOURCE_DIR}/blockchain" 
     DESTINATION "include" 
     FILES_MATCHING 
     PATTERN "*.h" 
)

install(TARGETS ${PROJECT_NAME}
     RUNTIME DESTINATION bin
     ARCHIVE DESTINATION lib
     LIBRARY DESTINATION lib)