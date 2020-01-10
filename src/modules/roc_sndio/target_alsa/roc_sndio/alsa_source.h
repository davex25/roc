/*
 * Copyright (c) 2019 Roc authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

//! @file roc_sndio/target_alsa/roc_sndio/alsa_source.h
//! @brief Alsa source


#ifndef ROC_SNDIO_ALSA_SOURCE_H_
#define ROC_SNDIO_ALSA_SOURCE_H_

#include "roc_sndio/isource.h"
#include "roc_sndio/config.h"

namespace roc {
namespace sndio {

class AlsaSource : public ISource, public core::NonCopyable<> {
public:
    AlsaSource();

    virtual ~AlsaSource();

    //! Get sample rate of an input file or a device.
    virtual size_t sample_rate() const;

    //! Check if the source has own clock.
    virtual bool has_clock() const;

    //! Get current source state.
    virtual State state() const;

    //! Wait until the source state becomes active.
    virtual void wait_active() const;

    //! Read frame.
    virtual bool read(audio::Frame&);
};

}
}
#endif

