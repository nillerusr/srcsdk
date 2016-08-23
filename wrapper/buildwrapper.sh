#$(ndk-which gcc) --sysroot=/home/a1ba/.android/android-ndk-r10e/platforms/android-19/arch-arm wrap.cpp -o wrap.o $@

#$(ndk-which gcc) --sysroot=/home/a1ba/.android/android-ndk-r10e/platforms/android-19/arch-arm -shared wrap.o -o libwrap.so -Xlinker --wrap=CommandLine_Tier0 -llog -ldl -L../../../origlibs -ltier0 -Wl,--no-undefined 

ndk-build APP_BUILD_SCRIPT="Android.mk" APP_ABI=armeabi-v7a-hard APP_PLATFORM=android-19 NDK_LIBS_OUT=libs
