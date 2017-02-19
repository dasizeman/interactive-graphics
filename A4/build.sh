#!/bin/bash
PROJECT_NAME=${PWD##*/}
THIS_DIR=`pwd`

if [ ! -d "../build" ]; then
    mkdir -p ../build
    cd ../build
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DPROJECT_DIR=${PROJECT_NAME} ..
    cd ${THIS_DIR}
fi

make -C ../build
cp ../build/Main .
