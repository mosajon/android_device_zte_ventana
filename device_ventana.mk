$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_full.mk)

# The gps config appropriate for this device
$(call inherit-product, device/common/gps/gps_cn_supl.mk)

## (1) First, the most specific values, i.e. the aspects that are specific to GSM
PRODUCT_COPY_FILES += \
    device/zte/ventana/init.rc:root/init.rc \
    device/zte/ventana/init.factory.rc:root/init.factory.rc \
    device/zte/ventana/init.factory.ventana.rc:root/init.factory.ventana.rc \
    device/zte/ventana/init.ventana.rc:root/init.ventana.rc \
    device/zte/ventana/init.ventana.usb.rc:root/init.ventana.usb.rc \
    device/zte/ventana/ueventd.rc:root/ueventd.rc \
    device/zte/ventana/ueventd.ventana.rc:root/ueventd.ventana.rc

## (2) Also get non-open-source GSM-specific aspects if available
$(call inherit-product-if-exists, vendor/zte/ventana/ventana-vendor.mk)

# zte helper scripts
PRODUCT_COPY_FILES += \
    device/zte/ventana/scripts/bluetooth.sh:system/bin/bluetooth.sh \
    device/zte/ventana/scripts/bluetooth_reset.sh:system/bin/bluetooth_reset.sh

## (3)  Finally, the least specific parts, i.e. the non-GSM-specific aspects

# Set en_US zh_CN as default locale
PRODUCT_LOCALES := zh_CN

# ventana uses high-density artwork where available
PRODUCT_LOCALES += hdpi

# copy all kernel modules under the "modules" directory to system/lib/modules
PRODUCT_COPY_FILES += $(shell \
    find device/zte/ventana/modules -name '*.ko' \
    | sed -r 's/^\/?(.*\/)([^/ ]+)$$/\1\2:system\/lib\/modules\/\2/' \
    | tr '\n' ' ')

DEVICE_PACKAGE_OVERLAYS += device/zte/ventana/overlay

LOCAL_PATH := device/zte/ventana
ifeq ($(TARGET_PREBUILT_KERNEL),)
	LOCAL_KERNEL := $(LOCAL_PATH)/kernel
else
	LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

PRODUCT_COPY_FILES += \
    $(LOCAL_KERNEL):kernel

$(call inherit-product, build/target/product/full_base.mk)

PRODUCT_PACKAGES += hcitool \
			hcidump \
			bluedroid \
			sdptool \
			setup_fs \
			bluez

# Set default property
#PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
#	ro.secure=0 \
#	ro.allow.mock.location=1 \
#	ro.debuggable=1 \
#	persist.sys.usb.config=mtp,adb

# Audio
PRODUCT_PACKAGES += \
    audio.a2dp.default \
    audio.primary.tegra \
    hwcomposer.default \
    libtinyalsa

# USB
PRODUCT_PACKAGES += \
    com.android.future.usb.accessory

# Invensense blob(s) necessary for Tilapia hardware
PRODUCT_PACKAGES += \
    libinvensense_hal

# system
PRODUCT_COPY_FILES += \
    device/zte/ventana/system/lib/libgnustl_shared.so:system/lib/libgnustl_shared.so \
    device/zte/ventana/system/lib/libhwr.so:system/lib/libhwr.so \
    device/zte/ventana/system/lib/libhwrword.so:system/lib/libhwrword.so \
    device/zte/ventana/system/lib/libhwrzhmodel.so:system/lib/libhwrzhmodel.so \
    device/zte/ventana/system/lib/libjni_delight.so:system/lib/libjni_delight.so \
    device/zte/ventana/system/lib/libjni_hmm_shared_engine.so:system/lib/libjni_hmm_shared_engine.so \
    device/zte/ventana/system/bin/dexopt-wrapper:system/bin/dexopt-wrapper \
    device/zte/ventana/system/app/U930Control.apk:system/app/U930Control.apk

#    device/zte/ventana/system/app/GooglePinyin.apk:system/app/GooglePinyin.apk \
#    device/zte/ventana/system/lib/libjni_googlepinyinime_latinime_5.so:system/lib/libjni_googlepinyinime_latinime_5.so \
#    device/zte/ventana/system/lib/libpinyin_data_bundle.so:system/lib/libpinyin_data_bundle.so \
#    device/zte/ventana/system/bin/hostapd:system/bin/hostapd \
#    device/zte/ventana/system/bin/hostapd_cli:system/bin/hostapd_cli \
#    device/zte/ventana/system/bin/wpa_cli:system/bin/wpa_cli \
#    device/zte/ventana/system/bin/wpa_supplicant:system/bin/wpa_supplicant \
#    device/zte/ventana/system/xbin/dhdutil:system/xbin/dhdutil

BOARD_WLAN_DEVICE_REV := bcm4330_b2
WIFI_BAND             := 802_11_ABG
$(call inherit-product-if-exists, hardware/broadcom/wlan/bcmdhd/firmware/bcm4330/device-bcm.mk)

# Board-specific init
PRODUCT_COPY_FILES += \
    device/zte/ventana/config/vold.fstab:system/etc/vold.fstab \
    device/zte/ventana/config/audio_policy.conf:system/etc/audio_policy.conf \
    device/zte/ventana/config/media_profiles.xml:system/etc/media_profiles.xml \
    device/zte/ventana/config/media_codecs.xml:system/etc/media_codecs.xml \
    device/zte/ventana/config/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf

#keyboard files
PRODUCT_COPY_FILES += \
    device/zte/ventana/keylayout/AVRCP.kl:system/usr/keylayout/AVRCP.kl \
    device/zte/ventana/keylayout/Bluetooth_00_06_66_42.kl:system/usr/keylayout/Bluetooth_00_06_66_42.kl \
    device/zte/ventana/keylayout/Generic.kl:system/usr/keylayout/Generic.kl \
    device/zte/ventana/keylayout/gpio-keys.kl:system/usr/keylayout/gpio-keys.kl \
    device/zte/ventana/keylayout/qwerty.kl:system/usr/keylayout/qwerty.kl \
    device/zte/ventana/keylayout/Vendor_044f_Product_d007.kl:system/usr/keylayout/Vendor_044f_Product_d007.kl \
    device/zte/ventana/keylayout/Vendor_045e_Product_028e.kl:system/usr/keylayout/Vendor_045e_Product_028e.kl \
    device/zte/ventana/keylayout/Vendor_045e_Product_0719.kl:system/usr/keylayout/Vendor_045e_Product_0719.kl \
    device/zte/ventana/keylayout/Vendor_046d_Product_c216.kl:system/usr/keylayout/Vendor_046d_Product_c216.kl \
    device/zte/ventana/keylayout/Vendor_046d_Product_c21d.kl:system/usr/keylayout/Vendor_046d_Product_c21d.kl \
    device/zte/ventana/keylayout/Vendor_046d_Product_c21f.kl:system/usr/keylayout/Vendor_046d_Product_c21f.kl \
    device/zte/ventana/keylayout/Vendor_046d_Product_c294.kl:system/usr/keylayout/Vendor_046d_Product_c294.kl \
    device/zte/ventana/keylayout/Vendor_046d_Product_c299.kl:system/usr/keylayout/Vendor_046d_Product_c299.kl \
    device/zte/ventana/keylayout/Vendor_046d_Product_c532.kl:system/usr/keylayout/Vendor_046d_Product_c532.kl \
    device/zte/ventana/keylayout/Vendor_054c_Product_0268.kl:system/usr/keylayout/Vendor_054c_Product_0268.kl \
    device/zte/ventana/keylayout/Vendor_057e_Product_0306.kl:system/usr/keylayout/Vendor_057e_Product_0306.kl \
    device/zte/ventana/keylayout/Vendor_05ac_Product_0239.kl:system/usr/keylayout/Vendor_05ac_Product_0239.kl \
    device/zte/ventana/keylayout/Vendor_22b8_Product_093d.kl:system/usr/keylayout/Vendor_22b8_Product_093d.kl \
    device/zte/ventana/keychars/Generic.kcm:system/usr/keychars/Generic.kcm \
    device/zte/ventana/keychars/qwerty2.kcm:system/usr/keychars/qwerty2.kcm \
    device/zte/ventana/keychars/qwerty.kcm:system/usr/keychars/qwerty.kcm \
    device/zte/ventana/keychars/Virtual.kcm:system/usr/keychars/Virtual.kcm \
    device/zte/ventana/bluetooth/bt_vendor.conf:system/etc/bluetooth/bt_vendor.conf \

# Permission files
PRODUCT_COPY_FILES += \
    device/zte/ventana/permissions/android.hardware.audio.low_latency.xml:system/etc/permissions/android.hardware.audio.low_latency.xml \
    device/zte/ventana/permissions/android.hardware.camera.autofocus.xml:system/etc/permissions/android.hardware.camera.autofocus.xml \
    device/zte/ventana/permissions/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    device/zte/ventana/permissions/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    device/zte/ventana/permissions/android.hardware.camera.xml:system/etc/permissions/android.hardware.camera.xml \
    device/zte/ventana/permissions/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    device/zte/ventana/permissions/android.hardware.location.xml:system/etc/permissions/android.hardware.location.xml \
    device/zte/ventana/permissions/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
    device/zte/ventana/permissions/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml \
    device/zte/ventana/permissions/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
    device/zte/ventana/permissions/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    device/zte/ventana/permissions/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
    device/zte/ventana/permissions/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    device/zte/ventana/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
    device/zte/ventana/permissions/android.hardware.touchscreen.multitouch.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.xml \
    device/zte/ventana/permissions/android.hardware.touchscreen.xml:system/etc/permissions/android.hardware.touchscreen.xml \
    device/zte/ventana/permissions/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
    device/zte/ventana/permissions/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml \
    device/zte/ventana/permissions/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml \
    device/zte/ventana/permissions/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    device/zte/ventana/permissions/android.software.live_wallpaper.xml:system/etc/permissions/android.software.live_wallpaper.xml \
    device/zte/ventana/permissions/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml \
    device/zte/ventana/permissions/android.software.sip.xml:system/etc/permissions/android.software.sip.xml \
    device/zte/ventana/permissions/com.android.location.provider.xml:system/etc/permissions/com.android.location.provider.xml \
    device/zte/ventana/permissions/com.google.widevine.software.drm.xml:system/etc/permissions/com.google.widevine.software.drm.xml \
    device/zte/ventana/permissions/features.xml:system/etc/permissions/features.xml \
    device/zte/ventana/permissions/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml

PRODUCT_BUILD_PROP_OVERRIDES += BUILD_UTC_DATE=0

PRODUCT_NAME := cm_ventana
PRODUCT_DEVICE := ventana
PRODUCT_MODEL := ZTE U930
