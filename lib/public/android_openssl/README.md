# Setup OpenSSL for Android Targets

Export the path to the Android NDK, e.g.

    export ANDROID_NDK_ROOT=~/Android/Sdk/ndk/21.3.6528147/
    export ANDROID_NDK_HOME=${ANDROID_NDK_ROOT}

Download the source,

    git clone https://github.com/openssl/openssl

create the target directories,

    mkdir include
    mkdir lib

and build for the different architectures.

## x86_64

    export PATH=$ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH

    mkdir include/x86_64
    mkdir lib/x86_64
    cd openssl/
    ./Configure shared android-x86_64 -D__ANDROID_API__=21
    make -j$(nproc) SHLIB_VERSION_NUMBER= SHLIB_EXT=_1_1.so build_libs
    mv libcrypto.a libssl.a ../lib/x86_64/
    make -j$(nproc) SHLIB_VERSION_NUMBER= SHLIB_EXT=_1_1.so build_sw
    cp -r include/openssl ../include/x86_64
    make clean
    cd ..

## arm64

    mkdir include/arm64
    mkdir lib/arm64
    cd openssl/
    ./Configure shared android-arm64 -D__ANDROID_API__=21
    make -j$(nproc) SHLIB_VERSION_NUMBER= SHLIB_EXT=_1_1.so build_libs
    mv libcrypto.a libssl.a ../lib/arm64
    make -j$(nproc) SHLIB_VERSION_NUMBER= SHLIB_EXT=_1_1.so build_sw
    cp -r include/openssl ../include/arm64
    make clean
    cd ..


