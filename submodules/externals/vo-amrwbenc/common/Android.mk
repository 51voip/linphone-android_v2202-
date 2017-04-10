LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)



LOCAL_SRC_FILES := cmnMemory.c

LOCAL_MODULE := libstagefright_enc_common

LOCAL_ARM_MODE := arm

LOCAL_STATIC_LIBRARIES :=

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/include

LOCAL_CFLAGS := $(VO_CFLAGS)

ifeq ($(VOTT), v5)
LOCAL_CFLAGS += -DARM -DASM_OPT
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/asm/ARMV5E
endif

ifeq ($(VOTT), v7)
LOCAL_CFLAGS += -DARM -DARMV7 -DASM_OPT
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/asm/ARMV5E
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/asm/ARMV7
endif

include $(BUILD_SHARED_LIBRARY)



