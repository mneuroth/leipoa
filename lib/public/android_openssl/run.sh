#!/usr/bin/env bash

rm -rf ./include
rm -rf ./lib

export PATH=$ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH

basepath=$PATH

for abi_arch in $(grep -v "^#" arch.lst); do

    abi=$(echo ${abi_arch} | cut -d\; -f 1)
    arch=$(echo ${abi_arch} | cut -d\; -f 2)

    echo "--------------------------------------------------------------------------------"
    echo "ARCHITECTURE: $arch"
    echo "--------------------------------------------------------------------------------"

    mkdir -p include/$abi
    mkdir -p lib/$abi

    cd openssl/
    make clean
    ./Configure shared $arch -D__ANDROID_API__=21
    make -j$(nproc) SHLIB_VERSION_NUMBER= SHLIB_EXT=_1_1.so build_libs
    mv libcrypto.a libssl.a ../lib/$abi/
    mv libcrypto.so libssl.so ../lib/$abi/
    make -j$(nproc) SHLIB_VERSION_NUMBER= bSHLIB_EXT=_1_1.so build_sw
    cp -r include/openssl ../include/$abi
    cd ..

done

