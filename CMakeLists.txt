cmake_minimum_required(VERSION 2.8)
project(libfc)  

# debuggery
if ($ENV{CLANG}) 
  message(STATUS "hey, you're using clang! good luck!")
  add_definitions(-g -O0 --std=c++0x --stdlib=libc++)
else ($ENV{CLANG})
  add_definitions(-g -O0 --std=c++0x)
  include(${CMAKE_CURRENT_SOURCE_DIR}/CodeCoverage.cmake)
endif($ENV{CLANG})

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
if ($ENV{CLANG}) 
  target_link_libraries (fc c++)
else ($ENV{CLANG})
  target_link_libraries (fc)
endif($ENV{CLANG})

if ($ENV{CLANG})
  message(STATUS "skipping unit tests, because you're using clang.")
else ($ENV{CLANG})
  file (GLOB UT_OBJ test/Test*.cpp)
  add_executable(fctest ${UT_OBJ})
  if (Boost_FOUND)
    target_link_libraries(fctest fc ${Boost_LIBRARIES} pcap)
  endif()
endif()

if ($ENV{CLANG})
  message(STATUS "skipping coverage tests, because you're using clang.")
else ($ENV{CLANG})
  setup_target_for_coverage(fccov-all fctest fccov)
  setup_target_for_coverage(fccov-transcoder fctest fccov --run_test=Transcoder)
  setup_target_for_coverage(fccov-loopfile fctest fccov --run_test=ImportExport)
  setup_target_for_coverage(fccov-messages fctest fccov --run_test=Messages)
endif()