cmake_minimum_required(VERSION 2.8)
project(libfc)

set (CMAKE_MODULE_PATH 
     "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Log4CPlus)
if (LOG4CPLUS_FOUND)
  include_directories(${Log4CPlus_INCLUDE_DIRS}) 
  add_definitions(-D_libfc_HAVE_LOG4CPLUS_)
else(LOG4CPLUS_FOUND)
  set(Log4CPlus_LIBRARIES "")
endif(LOG4CPLUS_FOUND)

find_package(Boost 1.42 COMPONENTS unit_test_framework REQUIRED)
if (Boost_FOUND)
  include_directories (${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)
 
find_package(Wandio REQUIRED)
if (WANDIO_FOUND)
  include_directories(${Wandio_INCLUDE_DIRS}) 
endif(WANDIO_FOUND)


# debuggery
if ($ENV{CLANG}) 
  message(STATUS "hey, you're using clang! good luck!")
  set(CMAKE_CXX_FLAGS "-g -Wall -Wno-invalid-offsetof --std=c++0x --stdlib=libc++ -O3")
  set(CMAKE_CXX_FLAGS_PROFILE "-g -Wall -Wno-invalid-offsetof --std=c++0x --stdlib=libc++ -O0 -pg")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wno-invalid-offsetof --std=c++0x --stdlib=libc++ -O0")
else ($ENV{CLANG})
  set(CMAKE_CXX_FLAGS "-g -Wall -Wno-invalid-offsetof --std=c++0x -O3")
  set(CMAKE_CXX_FLAGS_PROFILE "-g -Wall -Wno-invalid-offsetof --std=c++0x -O0 -pg")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wno-invalid-offsetof --std=c++0x -O0")
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    #include(${CMAKE_CURRENT_SOURCE_DIR}/CodeCoverage.cmake)
  else (CMAKE_BUILD_TYPE STREQUAL "Debug")
  endif (CMAKE_BUILD_TYPE STREQUAL "Debug")
  include_directories(lib)
endif($ENV{CLANG})
include_directories(.)


# Core objects; library classes begin with a upper-case letter,
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
file (GLOB FC_OBJ "lib/*.cpp")
file (GLOB EXCEPTIONS_OBJ "lib/exceptions/*.cpp") 

add_library (fc ${FC_OBJ} ${EXCEPTIONS_OBJ})

add_executable(ipfix2csv ipfix2csv.cpp)
target_link_libraries(ipfix2csv fc ${Boost_LIBRARIES}
                                ${Wandio_LIBRARIES}
                                ${Log4CPlus_LIBRARIES})

add_executable(cbinding cbinding.c)
target_link_libraries(cbinding fc ${Wandio_LIBRARIES})

if ($ENV{CLANG}) 
  target_link_libraries (fc c++)
else ($ENV{CLANG})
  target_link_libraries (fc ${Log4CPlus_LIBRARIES})
endif($ENV{CLANG})

if ($ENV{CLANG})
  message(STATUS "skipping unit tests, because you're using clang.")
else ($ENV{CLANG})
  file (GLOB UT_OBJ test/Test*.cpp)
  add_executable(fctest ${UT_OBJ})
  target_link_libraries(fctest fc ${Boost_LIBRARIES}
                                  ${Wandio_LIBRARIES}
                                  ${Log4CPlus_LIBRARIES})
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
  target_link_libraries(fcprof fc ${Boost_LIBRARIES}
                                ${Wandio_LIBRARIES} 
                               ${Log4CPlus_LIBRARIES})
endif()  

# Fcold.  We handle this in the same way we handle libfc and
# associated executables: we build a library of fcold objects and then
# link the fcold main function against that.  If yuo want to do this
# in another way, be my guest.
#
file (GLOB FCOLD_OBJ "fcold/*.cpp")

add_library (fcoldl ${FCOLD_OBJ})

add_executable (fcold fcold.cpp)
target_link_libraries(fcold fcoldl fc
                            ${Boost_LIBRARIES}
                            ${Wandio_LIBRARIES} 
                            ${Log4CPlus_LIBRARIES})