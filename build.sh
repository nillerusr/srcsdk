#!/bin/sh

export NDK_TOOLCHAIN_VERSION=4.8
make -f $2.mak $1 NDK=1 NDK_PATH=/home/a1ba/.android/android-ndk-r10e APP_API_LEVEL=21 CFG=debug NDK_VERBOSE=1 COPY_DLL_TO_SRV=0 GEN_SYM=true -j4
