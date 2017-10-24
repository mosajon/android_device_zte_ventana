## Specify phone tech before including full_phone
$(call inherit-product, vendor/cm/config/gsm.mk)

# Release name
PRODUCT_RELEASE_NAME := ventana

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit device configuration
$(call inherit-product, device/zte/ventana/device_ventana.mk)

    PRODUCT_COPY_FILES += \
        vendor/cm/prebuilt/common/bootanimation/540.zip:system/media/bootanimation.zip

## Device identifier. This must come after all inclusions
PRODUCT_DEVICE := ventana
PRODUCT_NAME := cm_ventana
PRODUCT_BRAND := generic
PRODUCT_MODEL := ZTE U930
PRODUCT_MANUFACTURER := ZTE
