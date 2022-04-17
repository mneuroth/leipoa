#!/usr/bin/env bash

rm -rf ./include
rm -rf ./lib

mkdir include
mkdir lib

function copy-built {
    local target=$1
    local builtdir=$2
    local config=$3
    mkdir -p ./include/${target}/${config}
    cp -r ${builtdir}-${config}/include/* ./include/${target}/${config}/
    cp qtmqtt/src/mqtt/*.h ./include/${target}/${config}/
    mkdir -p ./lib/${target}/${config}
    cp -r ${builtdir}-${config}/lib/*.a* ./lib/${target}/${config}/
    cp -r ${builtdir}-${config}/lib/*.so* ./lib/${target}/${config}/
}

target=android_arm64-v8a
builtdir=./qtmqtt/built/build-qtmqtt-Android_Qt_6_3_0_Clang_arm64_v8a
copy-built $target $builtdir Debug
copy-built $target $builtdir Release

target=android_x86_64
builtdir=./qtmqtt/built/build-qtmqtt-Android_Qt_6_3_0_Clang_x86_64
copy-built $target $builtdir Debug
copy-built $target $builtdir Release

target=gcc_64
builtdir=./qtmqtt/built/build-qtmqtt-Desktop_Qt_6_3_0_GCC_64bit
copy-built $target $builtdir Debug
copy-built $target $builtdir Release

