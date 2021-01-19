#!/bin/bash

export NDK_HOME=$(pwd)/ndk-binaries
export PATH=$PATH:$(pwd)/ndk-binaries
export LIBPATH=$(pwd)/srceng-android/libs/armeabi-v7a
export NDK_TOOLCHAIN_VERSION=4.9

build()
{
	PW=$(pwd)
	cd $1
	make NDK=1 NDK_PATH=$NDK_HOME APP_API_LEVEL=19 CFG=debug NDK_VERBOSE=1 -j$(nproc --all)
	cp $1 $LIBPATH && echo $1 Installed
	cd $PW
}


build main libmain.so
build gl4es libRegal.so
build vinterface_wrapper/client libclient.so
build vinterface_wrapper/server libserver.so
build srceng-android/jni/src/tierhook libtierhook.so
