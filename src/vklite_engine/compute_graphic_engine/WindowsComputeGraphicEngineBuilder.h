//
// Created by leixing on 2025/6/9.
//

#pragma once

#ifdef VKLITE_WINDOWS

#include "vklite/platform/windows/surface/WindowsGLFWSurfaceBuilder.h"
#include "vklite_engine/compute_graphic_engine/ComputeGraphicEngineBuilder.h"

namespace vklite {
    class WindowsComputeGraphicEngineBuilder {
    private: // fields

    public: // methods

    public: // static
        /**
         * preset
         */
        static ComputeGraphicEngineBuilder asDefault(GLFWwindow *window);
    };
} // vklite

#endif
