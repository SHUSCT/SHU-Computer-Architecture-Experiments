#!/bin/bash

# Get the first argument
build_type=$1

# If no argument is provided, default to Release
if [ -z "$build_type" ]; then
    build_type=Release
elif [ "$build_type" != "Release" ] && [ "$build_type" != "Debug" ]; then
    echo "Invalid build type. Please use 'Release' or 'Debug'."
    exit 1
fi

PROJ_DIR=$(pwd)
Yutils_DIR=$(pwd)/vendor/Yutils

## [Optional] Remove the existing build directory
# rm -rf $Yutils_DIR/build

mkdir -p $Yutils_DIR/build

cd $Yutils_DIR/build

# Specify the compiler to use
CXX=g++-13

# Build Yutils
#    -DBUILD_SHARE_LIBS=OFF: Build static library.
cmake .. -DCMAKE_BUILD_TYPE=$build_type -DBUILD_SHARED_LIBS=OFF

make -j $(nproc)

cd $PROJ_DIR

echo "Yutils Build finished successfully."