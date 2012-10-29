# Set build type here [Debug, Release]
set( CMAKE_BUILD_TYPE Release )

# Use clang++ since it produces developer friendly error messages
#et( CMAKE_C_COMPILER clang )
#set( CMAKE_CXX_COMPILER clang++ )

# OR use g++ for faster executables
set( CMAKE_C_COMPILER gcc )
set( CMAKE_CXX_COMPILER g++ )

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project( netlist-parser )

# Add support for C++11 features (available in clang++ / g++ >= 4.6)
#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" )

# Other flags
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fopenmp" )

# Add local headers and libraries
include_directories( "include" )

# Allow boost libraries
include_directories( "contrib/boost_1_51_0" )

# List headers in IDE workspace for convenience
file( GLOB_RECURSE ALL_INCLUDE "include/*" )

add_executable( main.out ${ALL_INCLUDE} src/main.cpp src/graph.cpp )

# Add linker dependencied here
#target_link_libraries( find-community -lpthread )