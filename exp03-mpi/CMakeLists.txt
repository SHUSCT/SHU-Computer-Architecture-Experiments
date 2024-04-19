project(Exp03)

find_package(MPI REQUIRED)
if (MPI_FOUND)
    message(STATUS "MPI Found")
    include_directories(${MPI_INCLUDE_PATH})
    link_libraries(${MPI_LIBRARIES})
else()
    message(FATAL_ERROR "MPI Not Found")
endif()


# Link to TBB if it is Intel MPI
if (MPI_CXX_COMPILER_ID STREQUAL "Intel")
    find_package(TBB REQUIRED)
    if (TBB_FOUND)
        message(STATUS "TBB Found")
        include_directories(${TBB_INCLUDE_DIRS})
        link_libraries(${TBB_LIBRARIES})
    else()
        message(FATAL_ERROR "TBB Not Found")
    endif()
else()
endif()

# Include Dirs
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../vendor/Yutils/include)

# Link to Yutils library automatically
link_libraries(Yutils)
# Or link to the library manually
# > link_libraries(/path/to/lib)

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

# hellompi
add_executable(hellompi ${CXX_SRCS} ${PROJECT_SOURCE_DIR}/src/hellompi_main.cpp)