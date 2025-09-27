cmake_minimum_required(VERSION 3.25.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(app_http)

file(GLOB files
     "*/*.cpp"
)

# First check the bundled copy that lives inside this repository so the
# library can build even when the system doesn't provide `httplib.h`.
set(_cppbase_httplib_hints
    "${CMAKE_CURRENT_SOURCE_DIR}/http"
    /usr/include
    /usr/local/include
)

find_path(HTTPLIB_INCLUDE_DIR httplib.h PATHS ${_cppbase_httplib_hints})

if(NOT HTTPLIB_INCLUDE_DIR)
    message(WARNING "httplib.h not found; building a header-only stub for app_http")
    add_library(${PROJECT_NAME} INTERFACE)
    target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_definitions(${PROJECT_NAME} INTERFACE APP_HTTP_MISSING_DEPENDENCIES)
    target_link_libraries(${PROJECT_NAME} INTERFACE app_utils)
    return()
endif()

add_library(${PROJECT_NAME} SHARED ${files})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/http
        ${HTTPLIB_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        app_utils
)

install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/http"
    DESTINATION "include"
    FILES_MATCHING
    PATTERN "*.h"
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)
