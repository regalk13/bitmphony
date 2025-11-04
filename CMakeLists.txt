cmake_minimum_required(VERSION 3.16)
project(bitmphony LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(GME QUIET IMPORTED_TARGET gme)
endif()

add_executable(bitmphony src/main.cpp)
target_link_libraries(bitmphony PRIVATE SDL2::SDL2
  $<IF:$<BOOL:${GME_FOUND}>,PkgConfig::GME,gme>)
