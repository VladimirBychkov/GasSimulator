cmake_minimum_required(VERSION 3.20.0)
project(GasSimulator LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

add_subdirectory(src)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
 
