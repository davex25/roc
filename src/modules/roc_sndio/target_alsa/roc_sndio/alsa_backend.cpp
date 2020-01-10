/*
 * Copyright (c) 2019 Roc authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "roc_core/stddefs.h"
#include "roc_core/unique_ptr.h"
#include "alsa_backend.h"
#include "alsa_sink.h"

namespace roc {
namespace sndio {

bool AlsaBackend::probe(const char* driver, const char* inout, int filter_flags) {
    printf("Probing alsa backend\n");
    return !driver || strcmp(driver, "alsanative") == 0;
}

ISink* AlsaBackend::open_sink(core::IAllocator& allocator, const char* driver, const char* output, const Config& config) {
    core::UniquePtr<AlsaSink> sink(new (allocator) AlsaSink(config),
                                         allocator);
    if (!sink) {
        return NULL;
    }

    return sink.release();
}

ISource* AlsaBackend::open_source(core::IAllocator& allocator,
                                 const char* driver,
                                 const char* input,
                                 const Config& config) {
    return NULL;
}

    //! Append supported dirvers to the list.
bool AlsaBackend::get_drivers(core::Array<DriverInfo>& arr, int filter_flags) {
    if (filter_flags & FilterDevice) {
        return add_driver_uniq(arr, "alsanative");
    }
}

}
}
