# Minimum required CMake version
cmake_minimum_required(VERSION 3.21)

# Project name 
project(SHU-CSAE)

# Set C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Set different compile options for Release and Debug modes
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /openmp")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /Od /openmp")
    # MSVC compiler-specific options
    add_compile_options(/permissive- /Zc:forScope)
# Else if GCC
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # Set different compile options for Release and Debug modes
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fopenmp")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -fopenmp")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -ccbin /usr/bin/g++-11")
else()
    message(FATAL_ERROR "Unsupported compiler")
endif()

# SUBDIRECTORIES >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Yutils Library -----------------------------------------------------------
option(SHARED_YUTILS "Build Yutils as a shared library" OFF)
if (SHARED_YUTILS)
    # Set BUILD_SHARED_LIBS to ON to build Yutils as a shared library
    set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libs for Yutils" FORCE)
    # Build Yutils as a shared library
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/Yutils)
else()
    # Set BUILD_SHARED_LIBS to OFF to build Yutils as a shared library
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libs for Yutils" FORCE)
    # Build Yutils as a static library
    add_subdirectory(vendor/Yutils)
endif()
# You can comment out some of the following subdirectories 
# if you don't want to build all of them
# --------------------------------------------------------------------------
# Experiment 02: OpenMP
add_subdirectory(exp02-openmp)
# SUBDIRECTORIES <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<