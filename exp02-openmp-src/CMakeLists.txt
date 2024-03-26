# Include Dirs
include_directories(${PROJECT_SOURCE_DIR}/include)

# Get all sources files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Remove all main files
list(FILTER SOURCES EXCLUDE REGEX ".*main.cpp$")

# Add executable with the project name
add_executable(hellomp.out ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/hellomp_main.cpp)
add_executable(matmul.out ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/matmul_main.cpp)
add_executable(computePi.out ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/computePi_main.cpp)