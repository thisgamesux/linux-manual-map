cmake_minimum_required(VERSION 2.8.4)

if (WIN32 OR APPLE)
	message(FATAL_ERROR "Windows or Mac OSX is not supported")
endif (WIN32 OR APPLE)

project(linux_manual_map)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
 
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

set(SOURCE_FILES 
${PROJECT_SOURCE_DIR}/src/main.cpp
${PROJECT_SOURCE_DIR}/src/FileSystem.cpp 
${PROJECT_SOURCE_DIR}/src/Handle.cpp 
${PROJECT_SOURCE_DIR}/src/Loader.cpp 
${PROJECT_SOURCE_DIR}/src/Region.cpp 
${PROJECT_SOURCE_DIR}/src/Module.cpp 
${PROJECT_SOURCE_DIR}/src/Elf.cpp 
${PROJECT_SOURCE_DIR}/src/Elf64.cpp 
${PROJECT_SOURCE_DIR}/src/Elf32.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(linux_manual_map ${SOURCE_FILES})

add_subdirectory(test)