cmake_minimum_required(VERSION 2.8.4)
project(linux_manual_map)

set(SOURCE_FILES main.cpp FileSystem.cpp Handle.cpp Loader.cpp Region.cpp Module.cpp Elf.cpp Elf64.cpp Elf32.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(linux_manual_map ${SOURCE_FILES})