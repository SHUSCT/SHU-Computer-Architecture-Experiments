# Include Dirs
include_directories(${PROJECT_SOURCE_DIR}/include)

project(Exp02 LANGUAGES CXX CUDA)
enable_language(CUDA)
# find_package(CUDA)
include(CheckLanguage)
check_language(CUDA)

# Get all sources files
file(
    GLOB_RECURSE SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
)
list(
    FILTER SOURCES EXCLUDE 
    REGEX ${CMAKE_CURRENT_SOURCE_DIR}/.*main.cpp$
)

# Set output directory
set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}
)


# hellomp
add_executable(hellomp ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/hellomp_main.cpp)

# matmul
add_executable(matmul ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/matmul_main.cpp)

# computePi
add_executable(computePi ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/computePi_main.cpp)

# sharedVars
add_executable(sharedVars ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/sharedVars_main.cpp)

# innerProduct
add_executable(
    innerProduct 
    ${SOURCES} 
    ${CMAKE_CURRENT_SOURCE_DIR}/innerProduct_main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/innerProduct.cu
)
set_target_properties(
    innerProduct
    PROPERTIES
    CUDA_STANDARD 14
)