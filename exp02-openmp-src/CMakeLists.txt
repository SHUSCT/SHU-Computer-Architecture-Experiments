# Include Dirs
include_directories(${PROJECT_SOURCE_DIR}/include)

# Get all sources files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(FILTER SOURCES EXCLUDE REGEX ".*main.cpp$")

# Set output directory
set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/bin/exp02/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}
)

# hellomp
add_executable(hellomp.out ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/hellomp_main.cpp)

# matmul
add_executable(matmul.out ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/matmul_main.cpp)

# computePi
add_executable(computePi.out ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/computePi_main.cpp)

# sharedVars
add_executable(sharedVars.out ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/sharedVars_main.cpp)