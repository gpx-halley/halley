LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := SandDemo
LOCAL_SRC_FILES := SandDemo.cpp
#LOCAL_SHARED_LIBRARIES := Libtest
LOCAL_LDLIBS := -llog -lz

include $(BUILD_SHARED_LIBRARY)
