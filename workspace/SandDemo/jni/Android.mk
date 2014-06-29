LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := SandDemo
LOCAL_SRC_FILES := SandDemo.c
#LOCAL_SHARED_LIBRARIES := Libtest   
#LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib ¨Cllog

include $(BUILD_SHARED_LIBRARY)
