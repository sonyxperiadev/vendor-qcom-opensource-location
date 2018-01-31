ifeq ($(PRODUCT_PLATFORM_SOD),true)
ifeq ($(TARGET_KERNEL_VERSION), 4.9)
LOCAL_PATH := $(call my-dir)
include $(LOCAL_PATH)/build/target_specific_features.mk
include $(call all-makefiles-under,$(LOCAL_PATH))
endif
endif

ifneq ($(filter rhine shinano,$(PRODUCT_PLATFORM)),)
include $(call all-subdir-makefiles)
endif
