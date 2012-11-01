cmake_minimum_required(VERSION 2.8)
project(libfc)

set(CMAKE_CXX_FLAGS_PROFILE "-O3 -pg")

# Use matched template cache?
#
# This has been shown in experiments to actually slow down the
# collection process for simple templates, when there are not many
# possible candidates for matching, so use this with care and make
# experiments before you commit to this flag.
#
# add_definitions(-DLIBFC_USE_MATCHED_TEMPLATE_CACHE)

# debuggery
if ($ENV{CLANG}) 
  message(STATUS "hey, you're using clang! good luck!")
  add_definitions(-g -Wall --std=c++0x --stdlib=libc++)
else ($ENV{CLANG})
  add_definitions(-g -Wall --std=c++0x)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    #include(${CMAKE_CURRENT_SOURCE_DIR}/CodeCoverage.cmake)
  endif (CMAKE_BUILD_TYPE STREQUAL "Debug")
endif($ENV{CLANG})
include_directories(.)

# include boost
find_package(Boost 1.42 COMPONENTS unit_test_framework REQUIRED)
if (Boost_FOUND)
  include_directories (${Boost_INCLUDE_DIRS})
endif()

# include log4cplus
include_directories(${Log4cplus_INCLUDE_DIR})
link_directories(${Log4cplus_LIBRARY_DIR})

# include pcap

# core objects; library classes begin with a upper-case letter,
# program files (containing main()) with a lower-case letter
file (GLOB FC_OBJ [A-Z]*.cpp)
file (GLOB EXCEPTIONS_OBJ exceptions/*.cpp) 

add_library (fc ${FC_OBJ} ${EXCEPTIONS_OBJ})
if ($ENV{CLANG}) 
  target_link_libraries (fc c++)
else ($ENV{CLANG})
  target_link_libraries (fc)
#  target_link_libraries (fc log4cplus)
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
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    #setup_target_for_coverage(fccov-all fctest fccov)
    #setup_target_for_coverage(fccov-transcoder fctest fccov --run_test=Transcoder)
    #setup_target_for_coverage(fccov-loopfile fctest fccov --run_test=ImportExport)
    #setup_target_for_coverage(fccov-messages fctest fccov --run_test=Messages)
  endif (CMAKE_BUILD_TYPE STREQUAL "Debug")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Profile")
  add_executable(fcprof fcprof.cpp test/TestCommon.cpp)
  target_link_libraries(fcprof fc ${Boost_LIBRARIES} pcap)
endif()  
