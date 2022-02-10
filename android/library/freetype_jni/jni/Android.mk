#
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
FREETYPE_ROOT=${LOCAL_PATH}/../../../../freetype-2.11.1/android-libs


LOCAL_LDLIBS += -llog -landroid
LOCAL_LDLIBS += -L${FREETYPE_ROOT}/$(TARGET_ARCH_ABI) -lfreetype

LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(realpath $(LOCAL_PATH)/..)
LOCAL_C_INCLUDES += $(FREETYPE_ROOT)/$(TARGET_ARCH_ABI)/include
LOCAL_C_INCLUDES += $(FREETYPE_ROOT)/$(TARGET_ARCH_ABI)/include/freetype2


LOCAL_SRC_FILES :=  ./textdraw.cpp \
					./ConvertUTF.c

LOCAL_MODULE    := TextDraw

include $(BUILD_SHARED_LIBRARY)
