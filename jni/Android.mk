LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := opengl-android
LOCAL_SRC_FILES := opengl-android.cpp

include $(BUILD_SHARED_LIBRARY)
