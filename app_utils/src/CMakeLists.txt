cmake_minimum_required(VERSION 3.25.2)

project(app_utils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE files
     "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp")

if(files STREQUAL "")
    message(WARNING "app_utils: no source files were found")
endif()

find_package(nlohmann_json QUIET)

if(NOT nlohmann_json_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        URL_HASH SHA256=d6c65aca6b1ed68e7a182f4757257b107ae403032760ed6ef121c9d55e81757d
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()

if(NOT TARGET nlohmann_json::nlohmann_json)
    message(FATAL_ERROR "nlohmann_json could not be located or fetched")
endif()

add_library(${PROJECT_NAME} SHARED ${files})
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        nlohmann_json::nlohmann_json
)

install(
     DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/utils"
     DESTINATION "include"
     FILES_MATCHING
     PATTERN "*.h"
)

install(TARGETS ${PROJECT_NAME}
     EXPORT app_utilsTargets
     RUNTIME DESTINATION bin
     ARCHIVE DESTINATION lib
     LIBRARY DESTINATION lib)