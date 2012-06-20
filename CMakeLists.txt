cmake_minimum_required(VERSION 2.8)
project(libfc)  

# debuggery
add_definitions(-g -O0 --std=c++0x --stdlib=libc++)

# include boost
find_package(Boost 1.42 COMPONENTS unit_test_framework REQUIRED)
if (Boost_FOUND)
  include_directories (. ${Boost_INCLUDE_DIRS})
endif()

# include pcap

# core objects
file (GLOB FC_OBJ *.cpp)
file (GLOB EXCEPTIONS_OBJ exceptions/*.cpp)

add_library (fc ${FC_OBJ} ${EXCEPTIONS_OBJ})
target_link_libraries (fc c++)

file (GLOB UT_OBJ test/Test*.cpp)

add_executable(fctest ${UT_OBJ})

if (Boost_FOUND)
  target_link_libraries(fctest fc ${Boost_LIBRARIES} pcap)
endif()
