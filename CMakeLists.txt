cmake_minimum_required(VERSION 3.12)

project(skiplist)

set(CXX_STANDARD 17)

include_directories()

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.cc")

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_executable(skiplist ${SOURCES})