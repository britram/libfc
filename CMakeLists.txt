cmake_minimum_required(VERSION 2.8)
project(libfc)

# Use matched template cache?
#
# This has been shown in experiments to actually slow down the
# collection process for simple templates, when there are not many
# possible candidates for matching, so use this with care and make
# experiments before you commit to this flag.
#
# add_definitions(-DLIBFC_USE_MATCHED_TEMPLATE_CACHE)

# Use log4cplus?
#
#add_definitions(-D_LIBFC_HAVE_LOG4CPLUS_)
#include_directories(/opt/log4cplus/include)
#link_directories(/opt/log4cplus/lib)

# debuggery
if ($ENV{CLANG}) 
  message(STATUS "hey, you're using clang! good luck!")
  set(CMAKE_CXX_FLAGS "-g -Wall --std=c++0x --stdlib=libc++ -O3")
  set(CMAKE_CXX_FLAGS_PROFILE "-g -Wall --std=c++0x --stdlib=libc++ -O0 -pg")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall --std=c++0x --stdlib=libc++ -O0")

else ($ENV{CLANG})
  set(CMAKE_CXX_FLAGS "-g -Wall --std=c++0x -O3")
  set(CMAKE_CXX_FLAGS_PROFILE "-g -Wall --std=c++0x -O0 -pg")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall --std=c++0x -O0")
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    #include(${CMAKE_CURRENT_SOURCE_DIR}/CodeCoverage.cmake)
  else (CMAKE_BUILD_TYPE STREQUAL "Debug")
  endif (CMAKE_BUILD_TYPE STREQUAL "Debug")
endif($ENV{CLANG})
include_directories(.)

# include boost
find_package(Boost 1.42 COMPONENTS unit_test_framework REQUIRED)
if (Boost_FOUND)
  include_directories (${Boost_INCLUDE_DIRS})
endif()


# core objects; library classes begin with a upper-case letter,
# program files (containing main()) with a lower-case letter.
# Unfortunately, either cmake or a library that's linked with cmake
# has managed to get the 30-year-old technique of globbing wrong (on
# the Mac). For example,
#
#   file (GLOB FC_OBJ "S*.cpp")
#
# yields Session.cpp and StructTemplate.cpp, as expected, whereas
#
#   file (GLOB FC_OBJ "[S]*.cpp")
#
# yields an empty list.  Therefore, the correct glob pattern
# [A-Z]*.cpp gives the wrong result and we will settle for *.cpp,
# which includes too much.  It must be said that this works perfectly
# fine on Linux.
#
file (GLOB FC_OBJ "*.cpp")
file (GLOB EXCEPTIONS_OBJ "exceptions/*.cpp") 

add_library (fc ${FC_OBJ} ${EXCEPTIONS_OBJ})

add_executable(ipfix2csv ipfix2csv.cpp)
target_link_libraries(ipfix2csv fc ${Boost_LIBRARIES} pcap)
#target_link_libraries(ipfix2csv fc ${Boost_LIBRARIES} pcap log4cplus)

add_executable(cbinding cbinding.c)
target_link_libraries(cbinding fc)

if ($ENV{CLANG}) 
  target_link_libraries (fc c++)
else ($ENV{CLANG})
  target_link_libraries (fc)
  #target_link_libraries (fc log4cplus)
endif($ENV{CLANG})

if ($ENV{CLANG})
  message(STATUS "skipping unit tests, because you're using clang.")
else ($ENV{CLANG})
  file (GLOB UT_OBJ test/Test*.cpp)
  add_executable(fctest ${UT_OBJ})
  target_link_libraries(fctest pcap)
  if (Boost_FOUND)
    target_link_libraries(fctest fc ${Boost_LIBRARIES})
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
