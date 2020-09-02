DLLNAME=libRegal
GL4ES_PATH=gl4es-git

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

DEFINES = -DNOX11 -DNO_GBM -DNOEGL -DNO_LOADER -DDEFAULT_ES=2 -DNO_INIT_CONSTRUCTOR -DANDROID

SRCS = $(GL4ES_PATH)/src/gl/arbconverter.c \
	$(GL4ES_PATH)/src/gl/arbgenerator.c \
	$(GL4ES_PATH)/src/gl/arbhelper.c \
	$(GL4ES_PATH)/src/gl/arbparser.c \
	$(GL4ES_PATH)/src/gl/array.c \
	$(GL4ES_PATH)/src/gl/blend.c \
	$(GL4ES_PATH)/src/gl/blit.c \
	$(GL4ES_PATH)/src/gl/buffers.c \
	$(GL4ES_PATH)/src/gl/build_info.c \
	$(GL4ES_PATH)/src/gl/debug.c \
	$(GL4ES_PATH)/src/gl/decompress.c \
	$(GL4ES_PATH)/src/gl/depth.c \
	$(GL4ES_PATH)/src/gl/directstate.c \
	$(GL4ES_PATH)/src/gl/drawing.c \
	$(GL4ES_PATH)/src/gl/enable.c \
	$(GL4ES_PATH)/src/gl/envvars.c \
	$(GL4ES_PATH)/src/gl/eval.c \
	$(GL4ES_PATH)/src/gl/face.c \
	$(GL4ES_PATH)/src/gl/fog.c \
	$(GL4ES_PATH)/src/gl/fpe.c \
	$(GL4ES_PATH)/src/gl/fpe_cache.c \
	$(GL4ES_PATH)/src/gl/fpe_shader.c \
	$(GL4ES_PATH)/src/gl/framebuffers.c \
	$(GL4ES_PATH)/src/gl/gl_lookup.c \
	$(GL4ES_PATH)/src/gl/getter.c \
	$(GL4ES_PATH)/src/gl/gl4es.c \
	$(GL4ES_PATH)/src/gl/glstate.c \
	$(GL4ES_PATH)/src/gl/hint.c \
	$(GL4ES_PATH)/src/gl/init.c \
	$(GL4ES_PATH)/src/gl/light.c \
	$(GL4ES_PATH)/src/gl/line.c \
	$(GL4ES_PATH)/src/gl/list.c \
	$(GL4ES_PATH)/src/gl/listdraw.c \
	$(GL4ES_PATH)/src/gl/listrl.c \
	$(GL4ES_PATH)/src/gl/loader.c \
	$(GL4ES_PATH)/src/gl/logs.c \
	$(GL4ES_PATH)/src/gl/matrix.c \
	$(GL4ES_PATH)/src/gl/matvec.c \
	$(GL4ES_PATH)/src/gl/oldprogram.c \
	$(GL4ES_PATH)/src/gl/pixel.c \
	$(GL4ES_PATH)/src/gl/planes.c \
	$(GL4ES_PATH)/src/gl/pointsprite.c \
	$(GL4ES_PATH)/src/gl/preproc.c \
	$(GL4ES_PATH)/src/gl/program.c \
	$(GL4ES_PATH)/src/gl/queries.c \
	$(GL4ES_PATH)/src/gl/raster.c \
	$(GL4ES_PATH)/src/gl/render.c \
	$(GL4ES_PATH)/src/gl/shader.c \
	$(GL4ES_PATH)/src/gl/shaderconv.c \
	$(GL4ES_PATH)/src/gl/shader_hacks.c \
	$(GL4ES_PATH)/src/gl/stack.c \
	$(GL4ES_PATH)/src/gl/stencil.c \
	$(GL4ES_PATH)/src/gl/string_utils.c \
	$(GL4ES_PATH)/src/gl/stubs.c \
	$(GL4ES_PATH)/src/gl/texenv.c \
	$(GL4ES_PATH)/src/gl/texgen.c \
	$(GL4ES_PATH)/src/gl/texture.c \
	$(GL4ES_PATH)/src/gl/texture_compressed.c \
	$(GL4ES_PATH)/src/gl/texture_params.c \
	$(GL4ES_PATH)/src/gl/texture_read.c \
	$(GL4ES_PATH)/src/gl/texture_3d.c \
	$(GL4ES_PATH)/src/gl/uniform.c \
	$(GL4ES_PATH)/src/gl/vertexattrib.c \
	$(GL4ES_PATH)/src/gl/wrap/gl4eswraps.c \
	$(GL4ES_PATH)/src/gl/wrap/gles.c \
	$(GL4ES_PATH)/src/gl/wrap/glstub.c \
	$(GL4ES_PATH)/src/gl/math/matheval.c \
	$(GL4ES_PATH)/src/glx/hardext.c \
	$(GL4ES_PATH)/src/glx/glx.c \
	$(GL4ES_PATH)/src/glx/lookup.c \
	$(GL4ES_PATH)/src/glx/gbm.c \
	$(GL4ES_PATH)/src/glx/streaming.c \
	eglwrap.c \

OBJS += $(patsubst %.c,%.o,$(SRCS))

LIBS =

OPTFLAGS=-O2 -march=armv7-a -mtune=cortex-a15 -mthumb -mfloat-abi=softfp -mfpu=neon -mcpu=cortex-a9 -pipe -mvectorize-with-neon-quad -fPIC -fvisibility=hidden \
	-fomit-frame-pointer -funsafe-math-optimizations -ftree-vectorize -fgraphite-identity -floop-interchange -funsafe-loop-optimizations -finline-limit=256

CFLAGS=$(OPTFLAGS) -funwind-tables -g -std=gnu99 -funwind-tables

INCLUDES= -include $(GL4ES_PATH)/include/android_debug.h -I $(GL4ES_PATH)/include

LDFLAGS = -shared -llog -march=armv7-a -mtune=cortex-a15 -mthumb -mfloat-abi=softfp -mfpu=neon -mcpu=cortex-a9 -pipe -mvectorize-with-neon-quad -fPIC -no-canonical-prefixes -Wl,--fix-cortex-a8 -Wl,--no-warn-mismatch -Wl,--no-undefined

ifeq ($(NDK),1)
	SYSROOT := "$(NDK_PATH)/platforms/android-$(APP_API_LEVEL)/arch-arm"
	CFLAGS += "--sysroot=$(SYSROOT)"
	CXXFLAGS += "--sysroot=$(SYSROOT)"
	INCLUDES += -I$(SYSROOT)/usr/include
	LDFLAGS += -L$(SYSROOT)/usr/lib -Wl,--build-id --sysroot=$(SYSROOT)
endif

%.o : %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c "$<" -o $(PWD)/"$@"

$(DLLNAME).so : $(OBJS)
	$(CXX) -o $(DLLNAME).so $(LDFLAGS) $(OBJS) $(LIBS)
clean:
	-rm -f $(OBJS)
	-rm -f $(DLLNAME).so
