#!/bin/sh

git submodule update --init --recursive

sh proto_gen.sh

# create the program directories:
# ./build will be the executable is compiled
# ./data will contain the user files
mkdir build data
# run cmake
cd build
cmake ../src -DDETACH_KERNEL_DRIVER=ON


