# Minimum required CMake version
cmake_minimum_required(VERSION 3.21)

# Project Name
project(Exp06)

# Set default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Set CUDA_STANDARD to be 14
set(CMAKE_CUDA_STANDARD 14)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Release flags
    string(
        APPEND CMAKE_CXX_FLAGS_RELEASE 
        "/permissive- /Zc:forScope /Zc:inline "
        "/fp:precise /O2 /openmp"
    )
    # Debug flags
    string(
        APPEND CMAKE_CXX_FLAGS_DEBUG
        "/permissive- /Zc:forScope /Zc:inline "
        "/fp:precise /Od /openmp /Zi"
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # Set different compile options for Release and Debug modes
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fopenmp")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -fopenmp")
    # [Note] 
    # | Use g++-11 for nvcc compatibility. 
    # | This must be set before CUDA language is enabled.
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -ccbin /usr/bin/g++-11")
else()
    message(FATAL_ERROR "Unsupported compiler")
endif()

# Link to Yutils library
# Include the header files
include_directories(${PROJECT_SOURCE_DIR}/../vendor/Yutils/include)
# [note] Change the path to the library file according to your environment
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    link_libraries(${PROJECT_SOURCE_DIR}/../vendor/Yutils/lib/libYutils.a)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    link_libraries(${PROJECT_SOURCE_DIR}/../vendor/Yutils/lib/Yutils.lib)
endif()

# Enable CUDA
enable_language(CUDA)

# Set output directory
string(
    CONCAT CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/
    ${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}/
)

# Add executable
add_executable(
    ABpC
    ${PROJECT_SOURCE_DIR}/src/ABpC_main.cpp
    ${PROJECT_SOURCE_DIR}/src/ABpC.cu
)