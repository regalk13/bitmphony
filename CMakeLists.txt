cmake_minimum_required(VERSION 3.16)
project(bitmphony LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
    pkg_check_modules(GME QUIET IMPORTED_TARGET gme)
endif()

add_subdirectory(libs/core)
add_subdirectory(apps/bitmphony)
