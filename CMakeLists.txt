cmake_minimum_required(VERSION 3.9)
project(rtspsvr VERSION 1.0 LANGUAGES C CXX)

# This project requires C++11.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

add_subdirectory(common)
add_subdirectory(test)
