# Setup OpenSSL for Android Targets

    git clone https://github.com/openssl/openssl

    mkdir include
    mkdir lib

    cp -r openssl/include/openssl ./include/

    mkdir lib/x86_64
    cd openssl/
    ./Configure shared android-x86_64 -D__ANDROID_API__=21
    make -j$(nproc) SHLIB_VERSION_NUMBER= SHLIB_EXT=_1_1.so build_libs
    mv libcrypto.a libssl.a ../lib/x86_64/
    make clean
    cd ..

    mkdir lib/arm64
    cd openssl/
    ./Configure shared android-arm64 -D__ANDROID_API__=21
    make -j$(nproc) SHLIB_VERSION_NUMBER= SHLIB_EXT=_1_1.so build_libs
    mv libcrypto.a libssl.a ../lib/arm64
    make clean
    cd ..


