/*
 * Copyright (c) 2019 Roc authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

//! @file roc_sndio/target_alsa/roc_sndio/alsa_sink.h
//! @brief Alsa sink


#ifndef ROC_SNDIO_ALSA_SINK_H_
#define ROC_SNDIO_ALSA_SINK_H_

#include <alsa/asoundlib.h>

#include "roc_sndio/isink.h"
#include "roc_sndio/config.h"

namespace roc {
namespace sndio {

class AlsaSink : public ISink, public core::NonCopyable<> {
public:
    AlsaSink(const Config& config);
    virtual ~AlsaSink();

    //! Get sample rate of the sink.
    virtual size_t sample_rate() const;

    //! Check if the sink has own clock.
    virtual bool has_clock() const;

    //! Write audio frame.
    virtual void write(audio::Frame& frame);

    bool check_valid_params();

private:
    snd_pcm_t* handle;
    const Config& config;
    snd_pcm_hw_params_t* params;
    FILE* testOutput;
};

}
}
#endif
