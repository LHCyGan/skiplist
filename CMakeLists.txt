cmake_minimum_required(VERSION 3.12)

project(skiplist)

set(CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.cc")

add_executable(skiplist ${SOURCES})