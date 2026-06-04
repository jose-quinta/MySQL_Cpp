cmake_minimum_required(VERSION 3.15)
project(MySQL_Cpp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── MySQL / MariaDB Connector/C ──────────────────────────────────────────
set(MYSQL_SEARCH_PATHS
    "C:/Program Files/MariaDB 12.3"
    "C:/Program Files/MariaDB 10.11"
    "C:/Program Files/MariaDB 10.6"
    "C:/Program Files/MySQL/MySQL Server 8.4"
    "C:/Program Files/MySQL/MySQL Server 8.0"
    "C:/Program Files/MySQL/MySQL Server 5.7"
    "C:/xampp/mysql"
)

find_path(MYSQL_INCLUDE_DIR
    NAMES mysql.h
    PATHS ${MYSQL_SEARCH_PATHS}
    PATH_SUFFIXES include include/mysql
    NO_DEFAULT_PATH
)

find_library(MYSQL_LIBRARY
    NAMES libmariadb mariadbclient libmysql mysqlclient
    PATHS ${MYSQL_SEARCH_PATHS}
    PATH_SUFFIXES lib
    NO_DEFAULT_PATH
)

if(NOT MYSQL_INCLUDE_DIR OR NOT MYSQL_LIBRARY)
    message(FATAL_ERROR
        "MySQL/MariaDB Connector/C not found.\n"
        "Usage:\n"
        "  cmake -DMYSQL_INCLUDE_DIR=\"C:/Path/to/include/mysql\" ^\n"
        "        -DMYSQL_LIBRARY=\"C:/Path/to/lib/libmariadb.lib\" ..\n"
        "Example (MariaDB 12.3):\n"
        "  cmake -DMYSQL_INCLUDE_DIR=\"C:/Program Files/MariaDB 12.3/include/mysql\" ^\n"
        "        -DMYSQL_LIBRARY=\"C:/Program Files/MariaDB 12.3/lib/libmariadb.lib\" .."
    )
endif()

message(STATUS "MySQL include: ${MYSQL_INCLUDE_DIR}")
message(STATUS "MySQL library: ${MYSQL_LIBRARY}")

# ── FLTK ─────────────────────────────────────────────────────────────────
# Set the path to the built FLTK
set(FLTK_DIR "C:/Users/User/Documents/Proyectos/Cpp/fltk/build"
    CACHE PATH "Path to FLTK build directory")

find_package(FLTK CONFIG REQUIRED)

# ── Sources ──────────────────────────────────────────────────────────────
set(SOURCES
    src/main.cpp
    src/db/Database.cpp
    src/model/Person.cpp
    src/repository/PersonRepository.cpp
    src/ui/MainWindow.cpp
)

# ── Executable ───────────────────────────────────────────────────────────
if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
else()
    add_executable(${PROJECT_NAME} ${SOURCES})
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${MYSQL_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${MYSQL_LIBRARY}
        fltk::fltk
        fltk::images
)
