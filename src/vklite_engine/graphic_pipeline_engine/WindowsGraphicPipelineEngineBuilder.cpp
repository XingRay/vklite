//
// Created by leixing on 2025/6/4.
//

#ifdef VKLITE_WINDOWS

#include "WindowsGraphicPipelineEngineBuilder.h"
#include "vklite/platform/windows/surface/WindowsGLFWSurfaceBuilder.h"
#include "vklite/platform/windows/plugin/WindowsGlfwSurfacePlugin.h"

namespace vklite {
    /**
     * preset
     */
    GraphicPipelineEngineBuilder WindowsGraphicPipelineEngineBuilder::asDefault(GLFWwindow *window) {
        GraphicPipelineEngineBuilder graphicPipelineEngineBuilder;

        graphicPipelineEngineBuilder.asDefault()
                .addInstancePlugin(SurfacePlugin::buildUnique())
                .addInstancePlugin(WindowsGlfwSurfacePlugin::buildUnique())
                .addDevicePlugin(SurfacePlugin::buildUnique())
                .addDevicePlugin(WindowsGlfwSurfacePlugin::buildUnique())
                .surfaceBuilder([=](const vklite::Instance &instance) {
                    return WindowsGLFWSurfaceBuilder()
                            .instance(instance.getVkInstance())
                            .window(window)
                            .buildUnique();
                });

        return std::move(graphicPipelineEngineBuilder);
    }
} // vklite

#endif
