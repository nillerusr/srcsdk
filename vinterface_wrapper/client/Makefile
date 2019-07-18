DLLNAME=libclient

ifeq ($(NDK),1)
	CC = $(shell $(NDK_PATH)/ndk-which gcc)
	CXX = $(shell $(NDK_PATH)/ndk-which g++)
	LD = $(shell $(NDK_PATH)/ndk-which ld)
	AR = $(shell $(NDK_PATH)/ndk-which ar)
	LINK = $(shell $(NDK_PATH)/ndk-which g++)
	STRIP = $(shell $(NDK_PATH)/ndk-which strip)
else
	CXX=g++
	CC=gcc
endif

DLL_SRCDIR=.

DEFINES = -DVPROF_LEVEL=1 -DGNUC -DNO_HOOK_MALLOC -DNO_MALLOC_OVERRIDE -DANDROID
DEFINES += -DVPC -DRAD_TELEMETRY_DISABLED -DNDEBUG -DGNUC -DPOSIX -DCOMPILER_GCC -D_DLL_EXT=.so -D_LINUX -DLINUX -DPOSIX -D_POSIX -DBINK_VIDEO -DGL_GLEXT_PROTOTYPES -DDX_TO_GL_ABSTRACTION -DUSE_SDL -DDEV_BUILD -DFRAME_POINTER_OMISSION_DISABLED -DSOURCE1=1 -D_LINUX=1 -D_POSIX=1 -DLINUX=1 -DPOSIX=1

SRCS = jni.cpp touch.cpp cdll_int.cpp ../wrapper.cpp
SRCS_C =
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
OBJS += $(patsubst %.c,%.o,$(SRCS_C))
LIBS =

OPTFLAGS=-march=armv7-a -mtune=cortex-a15 -mthumb -mfloat-abi=softfp -mfpu=neon -mcpu=cortex-a9 -pipe -mvectorize-with-neon-quad -g -fno-exceptions -fPIC
CXXFLAGS=$(OPTFLAGS) -fpermissive -std=gnu++0x -Wno-narrowing
CFLAGS=$(OPTFLAGS)

INCLUDES=-I. -I../ -I../../public/ -I../../public/tier0/ -I../../public/tier1/ -I../../common/ -I../../common/GameUI/
LDFLAGS = -shared -llog -march=armv7-a -mtune=cortex-a15 -mthumb -mfloat-abi=softfp -mfpu=neon -mcpu=cortex-a9 -pipe -mvectorize-with-neon-quad -fPIC -no-canonical-prefixes -Wl,--fix-cortex-a8 -Wl,--no-warn-mismatch -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -pthread

ifeq ($(NDK),1)
	SYSROOT := "$(NDK_PATH)/platforms/android-$(APP_API_LEVEL)/arch-arm"
	CFLAGS += "--sysroot=$(SYSROOT)"
	CXXFLAGS += "--sysroot=$(SYSROOT)"
	INCLUDES += -I$(NDK_PATH)/sources/cxx-stl/stlport/stlport/ -I$(NDK_PATH)/sources/android/support/include -I$(SYSROOT)/usr/include
	LIBS += "$(NDK_PATH)/sources/cxx-stl/stlport/libs/armeabi-v7a-hard/thumb/libstlport_static.a"
	LDFLAGS += -L$(SYSROOT)/usr/lib -Wl,--build-id --sysroot=$(SYSROOT)
else
	LIBS += $(HOME)/ndk-include/libs/libstlport_static.a
	INCLUDES += -I$(HOME)/ndk-include/cxx-stl/stlport/stlport/ -I$(HOME)/ndk-include//android/support/include/
endif

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c "$<" -o $(PWD)/"$@"

%.o : %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c "$<" -o $(PWD)/"$@"

$(DLLNAME).so : $(OBJS)
	$(CXX) -o $(DLLNAME).so $(LDFLAGS) $(OBJS) $(LIBS)
clean:
	-rm -f $(OBJS)
	-rm -f $(DLLNAME).so
