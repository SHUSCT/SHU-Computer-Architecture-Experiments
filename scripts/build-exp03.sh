#!/bin/bash
CXX=mpiicpx
build_type=Release
PROJECT_ROOT_DIR=$(pwd)/exp03-mpi
rm -rf $PROJECT_ROOT_DIR/build
mkdir -p $PROJECT_ROOT_DIR/build
cd $PROJECT_ROOT_DIR/build
cmake .. -DCMAKE_BUILD_TYPE=$build_type
make -j$(nproc)
cd $PROJECT_ROOT_DIR
echo "Build finished successfully."