DLLNAME=libclient

CXX=g++
CC=gcc

DLL_SRCDIR=.

DEFINES = -DVPROF_LEVEL=1 -DGNUC -DNO_HOOK_MALLOC -DNO_MALLOC_OVERRIDE -DANDROID
DEFINES += -DVPC -DRAD_TELEMETRY_DISABLED -DNDEBUG -DGNUC -DPOSIX -DCOMPILER_GCC -D_DLL_EXT=.so -D_LINUX -DLINUX -DPOSIX -D_POSIX -DDLLNAME=vinterface_cdll_wrap -DBINK_VIDEO -DGL_GLEXT_PROTOTYPES -DDX_TO_GL_ABSTRACTION -DUSE_SDL -DDEV_BUILD -DFRAME_POINTER_OMISSION_DISABLED -D_EXTERNAL_DLL_EXT=.so -DVPCGAMECAPS=VALVE -DPROJECTDIR=/home/den4ik/Desktop/srcsdk-master/vinterface_wrapper/client -D_DLL_EXT=.so -DSOURCE1=1 -DVPCGAME=valve -D_LINUX=1 -D_POSIX=1 -DLINUX=1 -DPOSIX=1

SRCS = jni.cpp touch.cpp cdll_int.cpp ../wrapper.cpp
SRCS_C =
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
OBJS += $(patsubst %.c,%.o,$(SRCS_C))
LIBS = $(HOME)/ndk-include/libs/libstlport_static.a

OPTFLAGS=-march=armv7-a -mtune=cortex-a15 -mthumb -mfloat-abi=softfp -mfpu=neon -mcpu=cortex-a9 -pipe -mvectorize-with-neon-quad -g -fno-exceptions
CXXFLAGS=$(OPTFLAGS) -fpermissive -std=gnu++0x -Wno-narrowing
CFLAGS=$(OPTFLAGS)

INCLUDES=-I. -I$(HOME)/ndk-include/cxx-stl/stlport/stlport/ -I../ -I../../public/ -I../../public/tier0/ -I../../public/tier1/ -I../../common/ -I../../common/GameUI/ -I$(HOME)/ndk-include/android/support/include/
LDFLAGS = -shared -llog -march=armv7-a -mtune=cortex-a15 -mthumb -mfloat-abi=softfp -mfpu=neon -mcpu=cortex-a9 -pipe -mvectorize-with-neon-quad -fPIC -no-canonical-prefixes -Wl,--fix-cortex-a8 -Wl,--no-warn-mismatch -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c "$<" -o $(PWD)/"$@"

%.o : %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c "$<" -o $(PWD)/"$@"

$(DLLNAME).so : $(OBJS)
	$(CXX) -o $(DLLNAME).so $(LDFLAGS) $(OBJS) $(LIBS)
clean:
	-rm -f $(OBJS)
	-rm -f $(DLLNAME).so
