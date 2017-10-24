/*
 * Copyright (C) 2011 The Android Open Source Project
 * Copyright (C) 2013 Thomas Wendt <thoemy@gmx.net>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "AudioWrapper"
//#define LOG_NDEBUG 0

#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/time.h>

#include <cutils/log.h>

#include "common.h"
#include "include/4.0/hardware/audio.h"

struct wrapper_audio_device {
    struct audio_hw_device device;
    struct wrapper::audio_hw_device *wrapped_device;
};

struct wrapper_stream_out {
    struct audio_stream_out stream;
    struct wrapper::audio_stream_out *wrapped_stream;
};

struct wrapper_stream_in {
    struct audio_stream_in stream;
    struct wrapper::audio_stream_in *wrapped_stream;
};


/**
 * device macros.
 */
#define WRAPPED_DEVICE(d) (((struct wrapper_audio_device*) d)->wrapped_device)

#define WRAPPED_DEVICE_CALL(d, func, ...) ({\
    WRAPPED_DEVICE(d)->func(WRAPPED_DEVICE(d), ##__VA_ARGS__);   \
})

#define RETURN_WRAPPED_DEVICE_CALL(d, func, ...) ({\
    ALOGV("%s", __FUNCTION__); \
    return WRAPPED_DEVICE(d)->func(WRAPPED_DEVICE(d), ##__VA_ARGS__); \
})

/**
 * Stream in macros.
 */
#define WRAPPED_STREAM_IN(p) (((struct wrapper_stream_in*) p)->wrapped_stream)
#define WRAPPED_STREAM_IN_COMMON(s) (WRAPPED_STREAM_IN(s)->common)
#define WRAPPED_STREAM_IN_COMMON_CALL(s, func, ...) ({\
    WRAPPED_STREAM_IN_COMMON(s).func(&WRAPPED_STREAM_IN_COMMON(s), ##__VA_ARGS__); \
})
#define RETURN_WRAPPED_STREAM_IN_CALL(s, func, ...) ({\
    ALOGV("%s", __FUNCTION__); \
    return WRAPPED_STREAM_IN(s)->func(WRAPPED_STREAM_IN(s), ##__VA_ARGS__); \
})
#define RETURN_WRAPPED_STREAM_IN_COMMON_CALL(s, func, ...) ({\
    ALOGV("%s", __FUNCTION__); \
    return WRAPPED_STREAM_IN_COMMON(s).func(&WRAPPED_STREAM_IN_COMMON(s), ##__VA_ARGS__); \
})


/**
 * Stream out macros.
 */
#define WRAPPED_STREAM_OUT(p) (((struct wrapper_stream_out*) p)->wrapped_stream)
#define WRAPPED_STREAM_OUT_COMMON(s) (WRAPPED_STREAM_OUT(s)->common)
#define WRAPPED_STREAM_OUT_COMMON_CALL(s, func, ...) ({\
    WRAPPED_STREAM_OUT_COMMON(s).func(&WRAPPED_STREAM_OUT_COMMON(s), ##__VA_ARGS__); \
})
#define RETURN_WRAPPED_STREAM_OUT_CALL(s, func, ...) ({\
    ALOGV("%s", __FUNCTION__); \
    return WRAPPED_STREAM_OUT(s)->func(WRAPPED_STREAM_OUT(s), ##__VA_ARGS__); \
})
#define RETURN_WRAPPED_STREAM_OUT_COMMON_CALL(s, func, ...) ({\
    ALOGV("%s", __FUNCTION__); \
    return WRAPPED_STREAM_OUT_COMMON(s).func(&WRAPPED_STREAM_OUT_COMMON(s), ##__VA_ARGS__); \
})

static uint32_t out_get_sample_rate(const struct audio_stream *stream)
{
    RETURN_WRAPPED_STREAM_OUT_COMMON_CALL(stream, get_sample_rate);
}

static int out_set_sample_rate(struct audio_stream *stream, uint32_t rate)
{
    RETURN_WRAPPED_STREAM_OUT_COMMON_CALL(stream, set_sample_rate, rate);
}

static size_t out_get_buffer_size(const struct audio_stream *stream)
{
    RETURN_WRAPPED_STREAM_OUT_COMMON_CALL(stream, get_buffer_size);
}

static audio_channel_mask_t out_get_channels(const struct audio_stream *stream)
{
    RETURN_WRAPPED_STREAM_OUT_COMMON_CALL(stream, get_channels);
}

static audio_format_t out_get_format(const struct audio_stream *stream)
{
    RETURN_WRAPPED_STREAM_OUT_COMMON_CALL(stream, get_format);
}

static int out_set_format(struct audio_stream *stream, audio_format_t format)
{
    RETURN_WRAPPED_STREAM_OUT_COMMON_CALL(stream, set_format, format);
}

static int out_standby(struct audio_stream *stream)
{
    RETURN_WRAPPED_STREAM_OUT_COMMON_CALL(stream, standby);
}

static int out_dump(const struct audio_stream *stream, int fd)
{
    RETURN_WRAPPED_STREAM_OUT_COMMON_CALL(stream, dump, fd);
}

static int out_set_parameters(struct audio_stream *stream, const char *kvpairs)
{
    ALOGI("%s: kvpairs: %s", __FUNCTION__, kvpairs);
    int ret;
    char * fixed_kvpairs = fixup_audio_parameters(kvpairs, JB_TO_ICS);
    ret = WRAPPED_STREAM_OUT_COMMON_CALL(stream, set_parameters, fixed_kvpairs);
    free(fixed_kvpairs);
    return ret;
}

static char * out_get_parameters(const struct audio_stream *stream, const char *keys)
{
    ALOGI("%s: keys: %s", __FUNCTION__, keys);
    char * kvpairs = WRAPPED_STREAM_OUT_COMMON_CALL(stream, get_parameters, keys);
    char * fixed_kvpairs = fixup_audio_parameters(kvpairs, ICS_TO_JB);
    free(kvpairs);
    return fixed_kvpairs;
}

static uint32_t out_get_latency(const struct audio_stream_out *stream)
{
    RETURN_WRAPPED_STREAM_OUT_CALL(stream, get_latency);
}

static int out_set_volume(struct audio_stream_out *stream, float left,
                          float right)
{
    RETURN_WRAPPED_STREAM_OUT_CALL(stream, set_volume, left, right);
}

static ssize_t out_write(struct audio_stream_out *stream, const void* buffer,
                         size_t bytes)
{
    RETURN_WRAPPED_STREAM_OUT_CALL(stream, write, buffer, bytes);
}

static int out_get_render_position(const struct audio_stream_out *stream,
                                   uint32_t *dsp_frames)
{
    RETURN_WRAPPED_STREAM_OUT_CALL(stream, get_render_position, dsp_frames);
}

static int out_add_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    RETURN_WRAPPED_STREAM_OUT_COMMON_CALL(stream, add_audio_effect, effect);
}

static int out_remove_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    RETURN_WRAPPED_STREAM_OUT_COMMON_CALL(stream, remove_audio_effect, effect);
}

#if 0
#ifndef ICS_AUDIO_BLOB
static int out_get_next_write_timestamp(const struct audio_stream_out *stream,
                                        int64_t *timestamp)
{
    ALOGD("%s", __FUNCTION__);
    return android::INVALID_OPERATION;
    //RETURN_WRAPPED_STREAM_OUT_CALL(stream, get_next_write_timestamp, timestamp);
}
#endif
#endif

/** audio_stream_in implementation **/
static uint32_t in_get_sample_rate(const struct audio_stream *stream)
{
    RETURN_WRAPPED_STREAM_IN_COMMON_CALL(stream, get_sample_rate);
}

static int in_set_sample_rate(struct audio_stream *stream, uint32_t rate)
{
    RETURN_WRAPPED_STREAM_IN_COMMON_CALL(stream, set_sample_rate, rate);
}

static size_t in_get_buffer_size(const struct audio_stream *stream)
{
    RETURN_WRAPPED_STREAM_IN_COMMON_CALL(stream, get_buffer_size);
}

static audio_channel_mask_t in_get_channels(const struct audio_stream *stream)
{
    RETURN_WRAPPED_STREAM_IN_COMMON_CALL(stream, get_channels);
}

static audio_format_t in_get_format(const struct audio_stream *stream)
{
    RETURN_WRAPPED_STREAM_IN_COMMON_CALL(stream, get_format);
}

static int in_set_format(struct audio_stream *stream, audio_format_t format)
{
    RETURN_WRAPPED_STREAM_IN_COMMON_CALL(stream, set_format, format);
}

static int in_standby(struct audio_stream *stream)
{
    RETURN_WRAPPED_STREAM_IN_COMMON_CALL(stream, standby);
}

static int in_dump(const struct audio_stream *stream, int fd)
{
    RETURN_WRAPPED_STREAM_IN_COMMON_CALL(stream, dump, fd);
}

static int in_set_parameters(struct audio_stream *stream, const char *kvpairs)
{
    ALOGI("%s: kvpairs: %s", __FUNCTION__, kvpairs);
    int ret;
    char * fixed_kvpairs = fixup_audio_parameters(kvpairs, JB_TO_ICS);
    ret = WRAPPED_STREAM_IN_COMMON_CALL(stream, set_parameters, fixed_kvpairs);
    free(fixed_kvpairs);
    return ret;
}

static char * in_get_parameters(const struct audio_stream *stream,
                                const char *keys)
{
    ALOGI("%s: keys: %s", __FUNCTION__, keys);
    char * kvpairs = WRAPPED_STREAM_IN_COMMON_CALL(stream, get_parameters, keys);
    char * fixed_kvpairs = fixup_audio_parameters(kvpairs, ICS_TO_JB);
    free(kvpairs);
    return fixed_kvpairs;
}

static int in_set_gain(struct audio_stream_in *stream, float gain)
{
    RETURN_WRAPPED_STREAM_IN_CALL(stream, set_gain, gain);
}

static ssize_t in_read(struct audio_stream_in *stream, void* buffer,
                       size_t bytes)
{
    RETURN_WRAPPED_STREAM_IN_CALL(stream, read, buffer, bytes);
}

static uint32_t in_get_input_frames_lost(struct audio_stream_in *stream)
{
    RETURN_WRAPPED_STREAM_IN_CALL(stream, get_input_frames_lost);
}

static int in_add_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    RETURN_WRAPPED_STREAM_IN_COMMON_CALL(stream, add_audio_effect, effect);
}

static int in_remove_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    RETURN_WRAPPED_STREAM_IN_COMMON_CALL(stream, remove_audio_effect, effect);
}

static int adev_open_output_stream(struct audio_hw_device *dev,
#ifndef ICS_AUDIO_BLOB
                              audio_io_handle_t handle,
                              audio_devices_t devices,
                              audio_output_flags_t flags,
                              struct audio_config *config,
                              struct audio_stream_out **stream_out)
#else
                              uint32_t devices,
                              int *format, uint32_t *channels,
                              uint32_t *sample_rate,
                              struct audio_stream_out **stream_out)
#endif
{
    struct wrapper_stream_out *out;
    int ret;
    ALOGI("%s: devices 0x%x", __FUNCTION__, devices);

    out = (struct wrapper_stream_out *)calloc(1, sizeof(struct wrapper_stream_out));
    if (!out)
        return -ENOMEM;


    devices = convert_audio_devices(devices, JB_TO_ICS);

#ifdef ICS_AUDIO_BLOB
    ret = WRAPPED_DEVICE_CALL(dev, open_output_stream, devices, format, channels, sample_rate,
                              &WRAPPED_STREAM_OUT(out));
#else
    ret = WRAPPED_DEVICE_CALL(dev, open_output_stream, devices, (int *)&config->format,
                              &config->channel_mask, &config->sample_rate,
                              &WRAPPED_STREAM_OUT(out));
#endif

    if(ret < 0)
        goto err_open;

    out->stream.common.get_sample_rate = out_get_sample_rate;
    out->stream.common.set_sample_rate = out_set_sample_rate;
    out->stream.common.get_buffer_size = out_get_buffer_size;
    out->stream.common.get_channels = out_get_channels;
    out->stream.common.get_format = out_get_format;
    out->stream.common.set_format = out_set_format;
    out->stream.common.standby = out_standby;
    out->stream.common.dump = out_dump;
    out->stream.common.set_parameters = out_set_parameters;
    out->stream.common.get_parameters = out_get_parameters;
    out->stream.common.add_audio_effect = out_add_audio_effect;
    out->stream.common.remove_audio_effect = out_remove_audio_effect;
    out->stream.get_latency = out_get_latency;
    out->stream.set_volume = out_set_volume;
    out->stream.write = out_write;
    out->stream.get_render_position = out_get_render_position;
#ifndef ICS_AUDIO_BLOB
    out->stream.get_next_write_timestamp = NULL; //out_get_next_write_timestamp;
#endif

    *stream_out = &out->stream;
    return 0;

err_open:
    free(out);
    *stream_out = NULL;
    return ret;
}

static void adev_close_output_stream(struct audio_hw_device *dev,
                                     struct audio_stream_out *stream)
{
    WRAPPED_DEVICE_CALL(dev, close_output_stream, WRAPPED_STREAM_OUT(stream));
    free(stream);
}

static int adev_set_parameters(struct audio_hw_device *dev, const char *kvpairs)
{
    ALOGI("%s: kvpairs: %s", __FUNCTION__, kvpairs);
    char *fixed_kvpairs = fixup_audio_parameters(kvpairs, JB_TO_ICS);
    int ret = WRAPPED_DEVICE_CALL(dev, set_parameters, fixed_kvpairs);
    free(fixed_kvpairs);
    return ret;
}

static char * adev_get_parameters(const struct audio_hw_device *dev,
                                  const char *keys)
{
    ALOGI("%s: keys: %s", __FUNCTION__, keys);
    char *kvpairs = WRAPPED_DEVICE_CALL(dev, get_parameters, keys);
    char *fixed_kvpairs = fixup_audio_parameters(kvpairs, ICS_TO_JB);
    free(kvpairs);
    return fixed_kvpairs;
}

static uint32_t adev_get_supported_devices(const struct audio_hw_device *dev)
{
    uint32_t devices = WRAPPED_DEVICE_CALL(dev, get_supported_devices);
    return convert_audio_devices(devices, ICS_TO_JB);
}

static int adev_init_check(const struct audio_hw_device *dev)
{
    RETURN_WRAPPED_DEVICE_CALL(dev, init_check);
}

static int adev_set_voice_volume(struct audio_hw_device *dev, float volume)
{
    RETURN_WRAPPED_DEVICE_CALL(dev, set_voice_volume, volume);
}

static int adev_set_master_volume(struct audio_hw_device *dev, float volume)
{
    RETURN_WRAPPED_DEVICE_CALL(dev, set_master_volume, volume);
}

#if 0
#ifndef ICS_AUDIO_BLOB
static int adev_get_master_volume(struct audio_hw_device *dev, float *volume)
{
    ALOGI("%s", __FUNCTION__);
    return 0;
    //RETURN_WRAPPED_DEVICE_CALL(dev, get_master_volume, volume);
}

static int adev_set_master_mute(struct audio_hw_device *dev, bool muted)
{
    ALOGI("%s", __FUNCTION__);
    return 0;
    //RETURN_WRAPPED_DEVICE_CALL(dev, set_master_mute, muted);
}

static int adev_get_master_mute(struct audio_hw_device *dev, bool *muted)
{
    ALOGI("%s", __FUNCTION__);
    return 0;
    //RETURN_WRAPPED_DEVICE_CALL(dev, get_master_mute, muted);
}
#endif
#endif

static int adev_set_mode(struct audio_hw_device *dev, audio_mode_t mode)
{
    RETURN_WRAPPED_DEVICE_CALL(dev, set_mode, mode);
}

static int adev_set_mic_mute(struct audio_hw_device *dev, bool state)
{
    RETURN_WRAPPED_DEVICE_CALL(dev, set_mic_mute, state);
}

static int adev_get_mic_mute(const struct audio_hw_device *dev, bool *state)
{
    RETURN_WRAPPED_DEVICE_CALL(dev, get_mic_mute, state);
}


#ifndef ICS_AUDIO_BLOB
static size_t adev_get_input_buffer_size(const struct audio_hw_device *dev,
                                    const struct audio_config *config)
{
    RETURN_WRAPPED_DEVICE_CALL(dev, get_input_buffer_size, config->sample_rate,
                               config->format, popcount(config->channel_mask));
}
#else
static size_t adev_get_input_buffer_size(const struct audio_hw_device *dev,
                                    uint32_t sample_rate, int format,
                                    int channel_count)
{
    RETURN_WRAPPED_DEVICE_CALL(dev, get_input_buffer_size, sample_rate, format,
                               channel_count);
}
#endif

#ifndef ICS_AUDIO_BLOB
static int adev_open_input_stream(struct audio_hw_device *dev,
                             audio_io_handle_t handle,
                             audio_devices_t devices,
                             struct audio_config *config,
                             struct audio_stream_in **stream_in)
#else
static int adev_open_input_stream(struct audio_hw_device *dev, uint32_t devices,
                             int *format, uint32_t *channels,
                             uint32_t *sample_rate,
                             audio_in_acoustics_t acoustics,
                             struct audio_stream_in **stream_in)
#endif
{
    struct wrapper_stream_in *in;
    int ret;

    ALOGI("%s: devices 0x%x", __FUNCTION__, devices);

    in = (struct wrapper_stream_in *)calloc(1, sizeof(struct wrapper_stream_in));
    if (!in)
        return -ENOMEM;

    devices = convert_audio_devices(devices, JB_TO_ICS);

#ifndef ICS_AUDIO_BLOB
    ret = WRAPPED_DEVICE_CALL(dev, open_input_stream, devices, (int *)&config->format,
                              &config->channel_mask, &config->sample_rate,
                              (audio_in_acoustics_t)0, &WRAPPED_STREAM_IN(in));
#else
    ret = WRAPPED_DEVICE_CALL(dev, open_input_stream, devices, format, channels,
                              sample_rate, acoustics, &WRAPPED_STREAM_IN(in));
#endif
    if(ret < 0)
        goto err_open;

    in->stream.common.get_sample_rate = in_get_sample_rate;
    in->stream.common.set_sample_rate = in_set_sample_rate;
    in->stream.common.get_buffer_size = in_get_buffer_size;
    in->stream.common.get_channels = in_get_channels;
    in->stream.common.get_format = in_get_format;
    in->stream.common.set_format = in_set_format;
    in->stream.common.standby = in_standby;
    in->stream.common.dump = in_dump;
    in->stream.common.set_parameters = in_set_parameters;
    in->stream.common.get_parameters = in_get_parameters;
    in->stream.common.add_audio_effect = in_add_audio_effect;
    in->stream.common.remove_audio_effect = in_remove_audio_effect;
    in->stream.set_gain = in_set_gain;
    in->stream.read = in_read;
    in->stream.get_input_frames_lost = in_get_input_frames_lost;

    *stream_in = &in->stream;
    return 0;

err_open:
    free(in);
    *stream_in = NULL;
    return ret;
}

static void adev_close_input_stream(struct audio_hw_device *dev,
                                   struct audio_stream_in *in)
{
    WRAPPED_DEVICE_CALL(dev, close_input_stream, WRAPPED_STREAM_IN(in));
    free(in);
}

static int adev_dump(const audio_hw_device_t *dev, int fd)
{
    RETURN_WRAPPED_DEVICE_CALL(dev, dump, fd);
}

static int adev_close(hw_device_t *dev)
{
    ALOGI("%s", __FUNCTION__);
    WRAPPED_DEVICE(dev)->common.close((hw_device_t*)&(WRAPPED_DEVICE(dev)));
    free(dev);
    return 0;
}

static int adev_open(const hw_module_t* module, const char* name,
                     hw_device_t** device)
{
    struct wrapper_audio_device *adev;
    int ret;

    ALOGI("Wrapping vendor audio primary");

    if (strcmp(name, AUDIO_HARDWARE_INTERFACE) != 0)
        return -EINVAL;

    adev = (struct wrapper_audio_device *) calloc(1, sizeof(struct wrapper_audio_device));
    if (!adev)
        return -ENOMEM;

    ret = load_vendor_module(module, name, (hw_device_t**) &adev->wrapped_device,
                             AUDIO_HARDWARE_MODULE_ID_PRIMARY);
    if(ret) {
        free(adev);
        return ret;
    }

    adev->device.common.tag = HARDWARE_DEVICE_TAG;
#ifndef ICS_AUDIO_BLOB
    adev->device.common.version = AUDIO_DEVICE_API_VERSION_2_0;
#else
    adev->device.common.version = AUDIO_DEVICE_API_VERSION_1_0;
#endif
    adev->device.common.module = (struct hw_module_t *) module;
    adev->device.common.close = adev_close;

    adev->device.get_supported_devices = adev_get_supported_devices;
    adev->device.init_check = adev_init_check;
    adev->device.set_voice_volume = adev_set_voice_volume;
    adev->device.set_master_volume = adev_set_master_volume;
#ifndef ICS_AUDIO_BLOB
    adev->device.get_master_volume = NULL; //adev_get_master_volume;
    adev->device.set_master_mute = NULL; //adev_set_master_mute;
    adev->device.get_master_mute = NULL; //adev_get_master_mute;
#endif
    adev->device.set_mode = adev_set_mode;
    adev->device.set_mic_mute = adev_set_mic_mute;
    adev->device.get_mic_mute = adev_get_mic_mute;
    adev->device.set_parameters = adev_set_parameters;
    adev->device.get_parameters = adev_get_parameters;
    adev->device.get_input_buffer_size = adev_get_input_buffer_size;
    adev->device.open_output_stream = adev_open_output_stream;
    adev->device.open_input_stream = adev_open_input_stream;
    adev->device.close_output_stream = adev_close_output_stream;
    adev->device.close_input_stream = adev_close_input_stream;
    adev->device.dump = adev_dump;

    *device = &adev->device.common;

    return 0;
}

static struct hw_module_methods_t hal_module_methods = {
    adev_open,
};

struct audio_module HAL_MODULE_INFO_SYM = {
    {
        HARDWARE_MODULE_TAG,
        AUDIO_MODULE_API_VERSION_0_1,
        HARDWARE_HAL_API_VERSION,
        AUDIO_HARDWARE_MODULE_ID,
        "LEGACY Audio HW Wrapper HAL",
        "The Android Open Source Project",
        &hal_module_methods,
        NULL,
        {0},
    },
};
