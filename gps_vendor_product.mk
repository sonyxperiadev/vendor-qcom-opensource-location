# vendor opensource packages
ifneq ($(BOARD_VENDOR_QCOM_GPS_LOC_API_HARDWARE),)

# Activate the following two lines for regression testing
#GNSS_SANITIZE := address cfi alignment bounds null unreachable integer
#GNSS_SANITIZE_DIAG := address cfi alignment bounds null unreachable integer

# Add product packages
#add QMI libraries for QMI targets
LOC_BOARD_PLATFORM_LIST += msm8953
LOC_BOARD_PLATFORM_LIST += msm8937
LOC_BOARD_PLATFORM_LIST += msm8998
LOC_BOARD_PLATFORM_LIST += apq8098_latv
LOC_BOARD_PLATFORM_LIST += sdm660
LOC_BOARD_PLATFORM_LIST += sdm845
LOC_BOARD_PLATFORM_LIST += msmpeafowl
LOC_BOARD_PLATFORM_LIST += sdm710
LOC_BOARD_PLATFORM_LIST += qcs605
LOC_BOARD_PLATFORM_LIST += msmnile
LOC_BOARD_PLATFORM_LIST += sdmshrike
LOC_BOARD_PLATFORM_LIST += $(MSMSTEPPE)
LOC_BOARD_PLATFORM_LIST += $(TRINKET)
LOC_BOARD_PLATFORM_LIST += kona
LOC_BOARD_PLATFORM_LIST += lito
LOC_BOARD_PLATFORM_LIST += bengal
LOC_BOARD_PLATFORM_LIST += atoll
LOC_BOARD_PLATFORM_LIST += lahaina
LOC_BOARD_PLATFORM_LIST += holi
LOC_BOARD_PLATFORM_LIST += taro

ifneq (,$(filter $(LOC_BOARD_PLATFORM_LIST),$(TARGET_BOARD_PLATFORM)))

PRODUCT_PACKAGES += libloc_api_v02
PRODUCT_PACKAGES += libgnsspps
PRODUCT_PACKAGES += libsynergy_loc_api
PRODUCT_PACKAGES += izat_remote_api_headers
PRODUCT_PACKAGES += loc_sll_if_headers

endif #is-board-platform-in-list

endif#BOARD_VENDOR_QCOM_GPS_LOC_API_HARDWARE
