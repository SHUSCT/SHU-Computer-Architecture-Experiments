#!/bin/bash
build_type=Release
Yutils_DIR=$(pwd)/vendor/Yutils
rm -rf $Yutils_DIR/build
mkdir -p $Yutils_DIR/build
cd $Yutils_DIR/build
CXX=g++-13 \
cmake .. -DCMAKE_BUILD_TYPE=$build_type -DBUILD_SHARED_LIBS=ON
make -j$(nproc)
cd $Yutils_DIR
echo "Yutils Build finished successfully."