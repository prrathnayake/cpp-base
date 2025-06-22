cmake_minimum_required(VERSION 3.25.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(app_database)

file(GLOB files
     "*/*.cpp"
)

find_package(PkgConfig REQUIRED)

# Find SQLite3 via pkg-config
pkg_check_modules(SQLITE3 REQUIRED sqlite3)
if(NOT SQLITE3_FOUND)
    message(FATAL_ERROR "Could not find SQLite3 via pkg-config")
endif()

# Find MySQL client library and includes manually
find_path(MYSQL_INCLUDE_DIR mysql.h
    PATHS /usr/include/mysql /usr/local/include/mysql /usr/include
)
find_library(MYSQLCLIENT_LIB mysqlclient
    PATHS /usr/lib /usr/lib/x86_64-linux-gnu /usr/local/lib
)

if(NOT MYSQL_INCLUDE_DIR OR NOT MYSQLCLIENT_LIB)
    message(FATAL_ERROR "Could not find MySQL client libraries/headers")
endif()

# Find PostgreSQL client library and includes via pkg-config
pkg_check_modules(POSTGRESQL REQUIRED libpq)
if(NOT POSTGRESQL_FOUND)
    message(FATAL_ERROR "Could not find PostgreSQL (libpq) via pkg-config")
endif()

# Add library target
add_library(${PROJECT_NAME} SHARED ${files})

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${SQLITE3_INCLUDE_DIRS}
    ${MYSQL_INCLUDE_DIR}
    ${POSTGRESQL_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SQLITE3_LIBRARIES}
    ${MYSQLCLIENT_LIB}
    ${POSTGRESQL_LIBRARIES}
)

# Install headers and library
install(
    DIRECTORY "${CMAKE_SOURCE_DIR}/database" 
    DESTINATION "include" 
    FILES_MATCHING PATTERN "*.h"
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
