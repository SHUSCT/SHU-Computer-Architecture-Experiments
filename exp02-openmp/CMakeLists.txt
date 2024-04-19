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

# Include Dirs
include_directories(${PROJECT_SOURCE_DIR}/include)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Set different compile options for Release and Debug modes
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /openmp")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /Od /openmp")
    # MSVC compiler-specific options
    add_compile_options(/permissive- /Zc:forScope)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # Set different compile options for Release and Debug modes
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fopenmp")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -fopenmp")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -ccbin /usr/bin/g++-11")
else()
    message(FATAL_ERROR "Unsupported compiler")
endif()

# Link to Yutils library
include_directories(${PROJECT_SOURCE_DIR}/../vendor/Yutils/include)
link_libraries(${PROJECT_SOURCE_DIR}/../vendor/Yutils/lib/libYutils.a)

# Get all sources files
file(
    GLOB_RECURSE CXX_SRCS 
    ${PROJECT_SOURCE_DIR}/src/*.cpp 
)
list(
    FILTER CXX_SRCS EXCLUDE 
    REGEX ${PROJECT_SOURCE_DIR}/src/.*main.cpp$
)

# Set output directory
string(
    CONCAT BIN_OUTPUT_PATH
    ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/
    ${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}/
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_PATH})

# hellomp
add_executable(hellomp ${CXX_SRCS} ${PROJECT_SOURCE_DIR}/src/hellomp_main.cpp)

# matmul
add_executable(matmul ${CXX_SRCS} ${PROJECT_SOURCE_DIR}/src/matmul_main.cpp)

# computePi
add_executable(computePi ${CXX_SRCS} ${PROJECT_SOURCE_DIR}/src/computePi_main.cpp)

# sharedVars
add_executable(sharedVars ${CXX_SRCS} ${PROJECT_SOURCE_DIR}/src/sharedVars_main.cpp)

# Try to enable CUDA
enable_language(CUDA)
if(CMAKE_CUDA_COMPILER)
    message(STATUS "CUDA found")
    # innerProduct
    add_executable(
        innerProduct 
        ${CXX_SRCS} 
        ${PROJECT_SOURCE_DIR}/src/innerProduct_main.cpp
        ${PROJECT_SOURCE_DIR}/src/innerProduct.cu
    )
    set_target_properties(
        innerProduct
        PROPERTIES
        CUDA_STANDARD 14
    )
else()
    message(STATUS "CUDA not found. Skip building CUDA examples.")
endif()