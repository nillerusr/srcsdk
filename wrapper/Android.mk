LOCAL_PATH := $(call my-dir)
APP_ABI := armeabi-v7a

include $(CLEAR_VARS)

LOCAL_MODULE    := wrap

LOCAL_CFLAGS += -fpermissive
#LOCAL_LDFLAGS += -Xlinker --wrap=CommandLine_Tier0

LOCAL_SRC_FILES := wrap_main.cpp wrap_cmdline.cpp

#LOCAL_LDLIBS := -llog -L$(LOCAL_PATH)/../../../origlibs -ltier0
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

