#!/bin/sh

export NDK_TOOLCHAIN_VERSION=4.9
make -f $2.mak $1 NDK=1 NDK_PATH=/home/a1ba/.android/android-ndk-r10e APP_API_LEVEL=19 CFG=debug -j4
