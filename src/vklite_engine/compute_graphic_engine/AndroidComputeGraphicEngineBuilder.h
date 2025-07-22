//
// Created by leixing on 2025/6/9.
//

#pragma once

#ifdef VKLITE_ANDROID

#include "vklite_engine/compute_graphic_engine/ComputeGraphicEngineBuilder.h"

namespace vklite {

    class AndroidComputeGraphicEngineBuilder {
    private:// fields

    public:// methods

    public: // static
        /**
         * preset
         */
        static ComputeGraphicEngineBuilder asDefault(ANativeWindow *window);
    };

} // vklite

#endif
