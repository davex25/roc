/*
 * Copyright (c) 2019 Roc authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "alsa_sink.h"

#include "roc_core/log.h"
#include "roc_audio/units.h"

#include <alsa/asoundlib.h>

namespace roc {

using namespace audio;
using namespace packet;

namespace sndio {

AlsaSink::AlsaSink(const Config& conf) : config(conf) {
    snd_output_t *log;
    int err = snd_output_stdio_attach(&log, stderr, 0);

    this->testOutput = fopen("/home/user/temp/pipelineout.pcm", "wb");

    // int rc = snd_pcm_open(&handle, "hw:3,0", SND_PCM_STREAM_PLAYBACK, 0);
    printf("Constructing sink\n");
    int rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        roc_log(LogError, "Unable to open alsa native device");
        return;
    }
    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(handle, params);

    printf("Constructing sink\n");
    printf("Allocated params\n");

    if (rc < 0) {
        roc_log(LogError, "Unable to open default alsa stream");
        return;
    }

    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_FLOAT_LE);
    int channelCount = num_channels(config.channels);
    printf("Channels: %d\n", channelCount);
    snd_pcm_hw_params_set_channels(handle, params, channelCount);
    unsigned int val = config.sample_rate == 0 ? 48000 : config.sample_rate;
    printf("Attempting to set sample rate to: %d\n", val);
    rc = snd_pcm_hw_params_set_rate_near(handle, params, &val, NULL);
    if (rc != 0) {
        roc_log(LogError, "Failed to set ALSA sample rate: %s", snd_strerror(rc));
        return;
    }
    snd_pcm_uframes_t frames = config.frame_size / channelCount;
    snd_pcm_hw_params_set_period_size_near(handle, params, &frames, NULL);
    rc = snd_pcm_hw_params(handle, params);
    if (rc != 0) {
        roc_log(LogError, "Unable to set hw parameters: %s", snd_strerror(rc));
        return;
    }

    snd_pcm_hw_params_get_period_size(params, &frames, NULL);
    printf("frame value set: %d\n", frames);
    snd_pcm_hw_params_get_period_time(params, &val, NULL);
    snd_pcm_prepare(handle);

    snd_pcm_hw_params_dump(params, log);
}

AlsaSink::~AlsaSink() {
    snd_pcm_drain(handle);
    snd_pcm_close(handle);
}

size_t AlsaSink::sample_rate() const {
    int rc;
    unsigned int rate = 0;
    int sanity=100;
    while (rate == 0 && sanity > 0) {
        rc = snd_pcm_hw_params_get_rate(params, &rate, NULL);
        if (rc != 0) {
            roc_log(LogError, "Unable to get sample rate from Alsa device: %s", snd_strerror(rc));
        } else if (rate == 0) {
            roc_log(LogInfo, "Sample rate not yet configured, retrying request");
        }
        --sanity;
    }
    printf("Hardware requested sample rate is: %d\n", rate);
    return (size_t)rate;
}

bool AlsaSink::has_clock() const {
    return false;
}

void AlsaSink::write(audio::Frame& frame) {
    size_t size = frame.size();
    void* buffer = frame.data();
    sample_t* bufPtr = (sample_t*)buffer;
    fwrite(buffer, sizeof(sample_t), frame.size(), testOutput);
    int rc = snd_pcm_writei(handle, buffer, frame.size()/2);
    if (rc == -EPIPE) {
        /* EPIPE means underrun */
        roc_log(LogError, "underrun occurred\n");
        printf("underrun occurred\n");
        snd_pcm_prepare(handle);
    } else if (rc < 0) {
        roc_log(LogError, "error from writei: %s\n", snd_strerror(rc));
        printf("error from writei: %s\n", snd_strerror(rc));
        return;
    }  else if (rc != (int)frame.size()/2) {
        roc_log(LogError, "short write, write %d frames\n", rc);
        printf("short write, write %d frames\n", rc);
        return;
    }
}

bool AlsaSink::check_valid_params() {
    return true;
}

}
}
