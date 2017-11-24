project(PackageTest CXX)
cmake_minimum_required(VERSION 3.8)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_CXX_STANDARD 17)

add_executable(example
    pup-fns.h
    example.cpp)
target_link_libraries(example CONAN_PKG::pupene)
