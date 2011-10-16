cmake_minimum_required(VERSION 2.8)
project(libfc)  

# debuggery
add_definitions(-g -O0)

# include boost
find_package( Boost 1.46 )
include_directories (. ${Boost_INCLUDE_DIRS})

# core objects
file (GLOB FC_OBJ *.cpp)
add_library (fc ${FC_OBJ})

file (GLOB TW_OBJ test/TestWriter.cpp test/TestCommon.cpp)
file (GLOB TR_OBJ test/TestReader.cpp test/TestCommon.cpp)

add_executable(fctest_write ${FC_OBJ} ${TW_OBJ})
