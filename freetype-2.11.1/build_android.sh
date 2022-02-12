#!/bin/bash

FFMPEG_EXT_PATH=`pwd`
NDK_PATH="/Users/pro21/Downloads/android-ndk-r21e/"
HOST_PLATFORM="darwin-x86_64"
ENABLED_DECODERS=("${@:4}")
JOBS=$(nproc 2> /dev/null || sysctl -n hw.ncpu 2> /dev/null || echo 4)
echo "Using $JOBS jobs for make"

COMMON_OPTIONS="
    --with-zlib=no
    --with-bzip2=no
    --with-png=no
    --with-harfbuzz=no
    --with-brotli=no
    --enable-static=no
    --enable-shared=yes
    --with-sysroot=${SYSROOT}
    "
TOOLCHAIN_PREFIX="${NDK_PATH}/toolchains/llvm/prebuilt/${HOST_PLATFORM}/bin"
SYSROOT="${NDK_PATH}/toolchains/llvm/prebuilt/${HOST_PLATFORM}/sysroot"

cd "${FFMPEG_EXT_PATH}"
make clean
FOLDER=`pwd`/android-libs/armeabi-v7a
mkdir -p ${FOLDER}
CC=${TOOLCHAIN_PREFIX}/armv7a-linux-androideabi16-clang \
./configure \
    --prefix=${FOLDER} \
    --libdir=${FOLDER} \
    --host=arm-linux-androideabi \
    ${COMMON_OPTIONS}
make -j$JOBS && make install

make clean

FOLDER=`pwd`/android-libs/arm64-v8a
mkdir -p ${FOLDER}
CC=${TOOLCHAIN_PREFIX}/aarch64-linux-android21-clang \
./configure \
    --prefix=${FOLDER} \
    --libdir=${FOLDER} \
    --host=aarch64-linux-android \
    ${COMMON_OPTIONS}
make -j$JOBS && make install

make clean
FOLDER=`pwd`/android-libs/x86
mkdir -p ${FOLDER}
CC=${TOOLCHAIN_PREFIX}/i686-linux-android16-clang \
./configure \
    --prefix=${FOLDER} \
    --libdir=${FOLDER} \
    --host=i686-linux-android \
    ${COMMON_OPTIONS}
make -j$JOBS && make install