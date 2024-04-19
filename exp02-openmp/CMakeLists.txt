project(Exp02)

enable_language(CUDA)
include(CheckLanguage)
check_language(CUDA)

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

# hellomp
add_executable(hellomp ${CXX_SRCS} ${PROJECT_SOURCE_DIR}/src/hellomp_main.cpp)

# matmul
add_executable(matmul ${CXX_SRCS} ${PROJECT_SOURCE_DIR}/src/matmul_main.cpp)

# computePi
add_executable(computePi ${CXX_SRCS} ${PROJECT_SOURCE_DIR}/src/computePi_main.cpp)

# sharedVars
add_executable(sharedVars ${CXX_SRCS} ${PROJECT_SOURCE_DIR}/src/sharedVars_main.cpp)

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