# Minimum required CMake version
cmake_minimum_required(VERSION 3.21)

project(Exp03)

# Set default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Dirs
include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(MPI REQUIRED)
if (MPI_FOUND)
    message(STATUS "MPI Found")
    include_directories(${MPI_INCLUDE_PATH})
    link_libraries(${MPI_LIBRARIES})
else()
    message(FATAL_ERROR "MPI Not Found")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -qopenmp")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -qopenmp")
    find_package(TBB REQUIRED)
    if (TBB_FOUND)
        message(STATUS "TBB Found")
        include_directories(${TBB_INCLUDE_DIRS})
        link_libraries(${TBB_LIBRARIES})
    else()
        message(FATAL_ERROR "TBB Not Found")
    endif()
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fopenmp")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -fopenmp")
endif()

# Link to Yutils library
include_directories(${PROJECT_SOURCE_DIR}/../vendor/Yutils/include)
link_libraries(${PROJECT_SOURCE_DIR}/../vendor/Yutils/lib/libYutils.a)

# Set output directory
string(
    CONCAT BIN_OUTPUT_PATH
    ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/
    ${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}/
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_PATH})

# hellompi
add_executable(hellompi ${PROJECT_SOURCE_DIR}/src/hellompi_main.cpp)

# matmul
add_executable(matmul ${PROJECT_SOURCE_DIR}/src/matmul_main.cpp)