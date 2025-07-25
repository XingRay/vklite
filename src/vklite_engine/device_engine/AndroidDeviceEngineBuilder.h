//
// Created by leixing on 2025/6/4.
//

#pragma once

#ifdef VKLITE_ANDROID

#include <memory>

#include "vklite_engine/device_engine/DeviceEngineBuilder.h"

namespace vklite {

    class AndroidDeviceEngineBuilder {
    private:// fields

    public:// methods

    public: // static
        /**
         * preset
         */
         [[nodiscard]]
        static DeviceEngineBuilder asDefault(ANativeWindow *window);

        [[nodiscard]]
        static std::unique_ptr<DeviceEngineBuilder> asDefaultUnique(ANativeWindow *window);
    };

} // vklite

#endif
