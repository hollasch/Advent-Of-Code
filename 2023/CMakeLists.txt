#---------------------------------------------------------------------------------------------------
# CMake Build Configuration for the 2023 Advent of Code Programs
#---------------------------------------------------------------------------------------------------

cmake_minimum_required ( VERSION 3.1.0...3.27.0 )

project ( AOC LANGUAGES CXX )

# Set to C++23
set ( CMAKE_CXX_STANDARD          23 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
set ( CMAKE_CXX_EXTENSIONS        OFF )

# Executables
add_executable(01a 01/01a.cpp )
