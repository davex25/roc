/*
 * Copyright (c) 2019 Roc authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

//! @file roc_sndio/target_alsa/roc_sndio/alsa_backend.h
//! @brief Alsa backend.

#include "roc_core/stddefs.h"
#include "roc_sndio/ibackend.h"

#ifndef ROC_SNDIO_ALSA_BACKEND_H_
#define ROC_SNDIO_ALSA_BACKEND_H_

namespace roc {
namespace sndio {

class AlsaBackend : public IBackend, core::NonCopyable<> {
public:
    static AlsaBackend& instance() {
        return core::Singleton<AlsaBackend>::instance();
    }

    //! Check whether the backend can handle given input or output.
    virtual bool probe(const char* driver, const char* inout, int filter_flags);

    //! Create and open a sink.
    virtual ISink* open_sink(core::IAllocator& allocator,
                             const char* driver,
                             const char* output,
                             const Config& config);

    //! Create and open a source.
    virtual ISource* open_source(core::IAllocator& allocator,
                                 const char* driver,
                                 const char* input,
                                 const Config& config);

    //! Append supported dirvers to the list.
    virtual bool get_drivers(core::Array<DriverInfo>& arr, int filter_flags);
};

}
}

#endif