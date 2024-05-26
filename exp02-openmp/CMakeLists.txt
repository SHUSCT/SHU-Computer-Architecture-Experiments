# Minimum required CMake version
cmake_minimum_required(VERSION 3.21)

# Project Name
project(Exp02)

# Set default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# Set output directory
string(
    CONCAT BIN_OUTPUT_PATH
    ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/
    ${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}/
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_PATH})

# hellomp
add_executable(hellomp ${PROJECT_SOURCE_DIR}/src/hellomp_main.cpp)

# matmul
add_executable(matmul ${PROJECT_SOURCE_DIR}/src/matmul_main.cpp)

# computePi
add_executable(computePi ${PROJECT_SOURCE_DIR}/src/computePi_main.cpp)

# sharedVars
add_executable(sharedVars ${PROJECT_SOURCE_DIR}/src/sharedVars_main.cpp)
