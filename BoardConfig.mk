USE_CAMERA_STUB := true

# Audio
BOARD_USES_GENERIC_AUDIO := false
#BOARD_USES_AUDIO_LEGACY := true
#TARGET_PROVIDES_LIBAUDIO := false

# inherit from the proprietary version
-include vendor/zte/ventana/BoardConfigVendor.mk

TARGET_NO_BOOTLOADER := true
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_CPU_VARIANT := cortex-a9
TARGET_ARCH := arm
ARCH_ARM_HAVE_ARMV7A := true
TARGET_ARCH_VARIANT := armv7-a
TARGET_ARCH_VARIANT_CPU := cortex-a9
TARGET_ARCH_VARIANT_FPU := vfpv3-d16
TARGET_CPU_SMP := true
TARGET_BOARD_PLATFORM := tegra
TARGET_BOOTLOADER_BOARD_NAME := ventana
TARGET_USERIMAGES_USE_EXT4 := true

BOARD_HAS_SDCARD_INTERNAL := true

#TARGET_SPECIFIC_HEADER_PATH := device/zte/ventana/include

BOARD_KERNEL_CMDLINE := 
BOARD_KERNEL_BASE := 0x10000000
BOARD_KERNEL_PAGESIZE := 2048

# fix this up by examining /proc/mtd on a running device
BOARD_BOOTIMAGE_PARTITION_SIZE := 0x00800000
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 0x00600000
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 536870912
#BOARD_USERDATAIMAGE_PARTITION_SIZE := 1834975232
BOARD_USERDATAIMAGE_PARTITION_SIZE := 183500800
BOARD_FLASH_BLOCK_SIZE := 4096

# Try to build the kernel
TARGET_KERNEL_CONFIG := tegra_android_defconfig

#TARGET_PREBUILT_KERNEL := device/zte/ventana/kernel

# Below is a sample of how you can tweak the mount points using the board config.
BOARD_HAS_NO_MISC_PARTITION := false
BOARD_RECOVERY_IGNORE_BOOTABLES := true
BOARD_DATA_DEVICE := /dev/block/mmcblk0p8
BOARD_DATA_FILESYSTEM := ext4
BOARD_DATA_FILESYSTEM_OPTIONS := wait noatime nosuid nodev journal_async_commit,data=writeback,nodelalloc,errors=panic
BOARD_SYSTEM_DEVICE := /dev/block/mmcblk0p3
BOARD_SYSTEM_FILESYSTEM_OPTIONS := wait ro
BOARD_SYSTEM_FILESYSTEM := ext4
BOARD_CACHE_DEVICE := /dev/block/mmcblk0p4
BOARD_CACHE_FILESYSTEM := ext4
BOARD_CACHE_FILESYSTEM_OPTIONS := wait noatime nosuid nodev journal_async_commit,data=writeback,nodelalloc,errors=panic

BOARD_USE_USB_MASS_STORAGE_SWITCH := true

# vold
BOARD_VOLD_MAX_PARTITIONS := 20
BOARD_VOLD_EMMC_SHARES_DEV_MAJOR := true

# Lights
TARGET_PROVIDES_LIBLIGHTS := true

# mediaplayer
#BOARD_USES_HW_MEDIARECORDER := true
#BOARD_USES_HW_MEDIASCANNER := false
#BOARD_USES_HW_MEDIAPLUGINS := true

#TARGET_OVERLAY_ALWAYS_DETERMINES_FORMAT := true
#TARGET_USES_GL_VENDOR_EXTENSIONS := true
#TARGET_ELECTRONBEAM_FRAMES := 15
#BOARD_USE_SCREENCAP := true

# Wi-Fi
#WPA_SUPPLICANT_VERSION := VER_0_6_X
#BOARD_WPA_SUPPLICANT_DRIVER := WEXT
#BOARD_HOSTAPD_DRIVER        := WEXT
#BOARD_WLAN_DEVICE := bcmdhd
#WIFI_DRIVER_MODULE_PATH     := "/system/lib/modules/bcmdhd.ko"
#WIFI_DRIVER_FW_STA_PATH     := "/system/vendor/firmware/bcm4330/fw_bcmdhd.bin"
#WIFI_DRIVER_FW_AP_PATH      := "/system/vendor/firmware/bcm4330/fw_bcmdhd_apsta.bin"
#WIFI_DRIVER_FW_PATH_P2P     := "/system/vendor/firmware/bcm4330/fw_bcmdhd_p2p.bin"
#WIFI_DRIVER_MODULE_NAME     := "bcmdhd"
#WIFI_DRIVER_MODULE_ARG      := "firmware_path=/system/vendor/firmware/bcm4330/fw_bcmdhd.bin nvram_path=/system/etc/nvram_4330.txt"

BOARD_WPA_SUPPLICANT_DRIVER := NL80211
WPA_SUPPLICANT_VERSION      := VER_0_8_X
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_bcmdhd
BOARD_HOSTAPD_DRIVER        := NL80211
BOARD_HOSTAPD_PRIVATE_LIB   := lib_driver_cmd_bcmdhd
BOARD_WLAN_DEVICE           := bcmdhd
WIFI_DRIVER_FW_PATH_PARAM   := "/sys/module/bcmdhd/parameters/firmware_path"
#WIFI_DRIVER_MODULE_PATH     := "/system/lib/modules/bcmdhd.ko"
WIFI_DRIVER_FW_PATH_STA     := "/vendor/firmware/fw_bcmdhd.bin"
WIFI_DRIVER_FW_PATH_P2P     := "/vendor/firmware/fw_bcmdhd_p2p.bin"
WIFI_DRIVER_FW_PATH_AP      := "/vendor/firmware/fw_bcmdhd_apsta.bin"

BOARD_MOBILEDATA_INTERFACE_NAME := "pdp0"

# Sensors invensense
BOARD_USES_GENERIC_INVENSENSE := true

# Bluetooth
BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_BCM := true

# Bluetooth
BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_BCM := true
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR ?= device/zte/ventana/bluetooth

#TARGET_OVERLAY_ALWAYS_DETERMINES_FORMAT := true

#TARGET_DONT_SET_AUDIO_AAC_FORMAT := true

#EGL
BOARD_EGL_CFG := device/zte/ventana/config/egl.cfg
USE_OPENGL_RENDERER := true

#HDMI
#BOARD_USES_LGE_HDMI_ROTATION := true

#COMMON_GLOBAL_CFLAGS += -DICS_AUDIO_BLOB -DICS_CAMERA_BLOB
COMMON_GLOBAL_CFLAGS += -DICS_AUDIO_BLOB
COMMON_GLOBAL_CFLAGS += -DDISABLE_HW_ID_MATCH_CHECK
DISABLE_HW_ID_MATCH_CHECK :=true

BOARD_HAS_NO_SELECT_BUTTON := true
# Use this flag if the board has a ext4 partition larger than 2gb
#BOARD_HAS_LARGE_FILESYSTEM := true
TARGET_RECOVERY_FSTAB := device/zte/ventana/recovery.fstab
TARGET_RECOVERY_INITRC := device/zte/ventana/recovery/init.rc
TARGET_USERIMAGES_USE_EXT4 := true

# Coustom Tools
TARGET_RELEASETOOL_OTA_FROM_TARGET_SCRIPT := device/zte/ventana/releasetools/ota_from_target_files

TARGET_SCREEN_HEIGHT := 960
TARGET_SCREEN_WIDTH := 540

ADDITIONAL_DEFAULT_PROPERTIES += ro.secure=0
ADDITIONAL_DEFAULT_PROPERTIES += ro.allow.mock.location=1
ADDITIONAL_DEFAULT_PROPERTIES += ro.debuggable=1
ADDITIONAL_DEFAULT_PROPERTIES += persist.sys.usb.config=mtp
