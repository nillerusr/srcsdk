#!/bin/sh

make -f games.mak $1 NDK=1 NDK_PATH=/home/a1ba/.android/android-ndk-r10e APP_API_LEVEL=21 -j4
