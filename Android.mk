ifneq ($(filter yukon rhine shinano kanuti kitakami loire tone yoshino,$(PRODUCT_PLATFORM)),)
include $(call all-subdir-makefiles)
endif
