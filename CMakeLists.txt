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

#file (GLOB TW_OBJ test/TestWriter.cpp test/TestCommon.cpp)
#file (GLOB TR_OBJ test/TestReader.cpp test/TestCommon.cpp)
#file (GLOB FR_OBJ test/FlowReader.cpp test/TestCommon.cpp)
#file (GLOB CF_OBJ test/Capfix.cpp test/TestCommon.cpp)

file (GLOB UT_OBJ test/Test*.cpp)

#add_executable(fctest_write ${TW_OBJ})
#add_executable(fctest_read ${TR_OBJ})
#add_executable(fcflowprint ${FR_OBJ})
#add_executable(fccapfix ${CF_OBJ})

add_executable(fctest ${UT_OBJ})

#target_link_libraries(fctest_write fc)
#target_link_libraries(fctest_read fc)
#target_link_libraries(fcflowprint fc)
#target_link_libraries(fccapfix fc pcap)

if (Boost_FOUND)
  target_link_libraries(fctest fc ${Boost_LIBRARIES} pcap)
endif()
