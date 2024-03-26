# Include Dirs
include_directories(${PROJECT_SOURCE_DIR}/include)


# Get all sources files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Remove all main files
list(FILTER SOURCES EXCLUDE REGEX ".*main.cpp$")

# hellomp
add_executable(hellomp.out ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/hellomp_main.cpp)
set_target_properties(
    hellomp.out PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/bin/exp02/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}
)

# matmul
add_executable(matmul.out ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/matmul_main.cpp)
set_target_properties(
    matmul.out PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/bin/exp02/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}
)
# Try find TBB lib
# This is essential for matmul if you installed Intel OneAPI on your machine
find_package(TBB REQUIRED)
if(TBB_FOUND)
    target_link_libraries(matmul.out ${TBB_IMPORTED_TARGETS})
endif()

# computePi
add_executable(computePi.out ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/computePi_main.cpp)
set_target_properties(
    computePi.out PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/bin/exp02/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}
)
