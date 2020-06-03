ifneq ($(BUILD_TINY_ANDROID),true)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libloc_loader

LOCAL_MODULE_TAGS := optional

LOCAL_SHARED_LIBRARIES := \
    liblog

LOCAL_SRC_FILES += \
    libloc_loader.c

LOCAL_HEADER_LIBRARIES := libcutils_headers libutils_headers

ifeq ($(TARGET_KERNEL_VERSION),$(filter $(TARGET_KERNEL_VERSION),3.18 4.4 4.9))
LOCAL_CFLAGS += -DUSE_QSOCKET
LOCAL_HEADER_LIBRARIES += \
    device_kernel_headers
endif

LOCAL_PROPRIETARY_MODULE := true

include $(CLEAR_VARS)
LOCAL_MODULE := libloc_loader_headers
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
include $(BUILD_HEADER_LIBRARY)

include $(BUILD_SHARED_LIBRARY)

endif # not BUILD_TINY_ANDROID
