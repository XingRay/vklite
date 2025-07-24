//
// Created by leixing on 2025/6/4.
//

#pragma once

#ifdef VKLITE_WINDOWS

#include "vklite/platform/windows/surface/WindowsGLFWSurfaceBuilder.h"
#include "vklite_engine/graphic_pipeline_engine/GraphicPipelineEngineBuilder.h"

namespace vklite {
    #
    class WindowsGraphicPipelineEngineBuilder {
    private: // fields

    public: // methods

    public: // static
        /**
         * preset
         */
        static GraphicPipelineEngineBuilder asDefault(GLFWwindow *window);
    };
} // vklite

#endif
