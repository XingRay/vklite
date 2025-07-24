//
// Created by leixing on 2025/6/9.
//

#ifdef VKLITE_WINDOWS

#include "WindowsComputeGraphicEngineBuilder.h"

#include "vklite/platform/windows/plugin/WindowsGlfwSurfacePlugin.h"

namespace vklite {
    /**
     * preset
     */
    ComputeGraphicEngineBuilder WindowsComputeGraphicEngineBuilder::asDefault(GLFWwindow *window) {
        ComputeGraphicEngineBuilder computePipelineEngineBuilder;

        computePipelineEngineBuilder.asDefault()
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

        return std::move(computePipelineEngineBuilder);
    }
} // vklite
#endif
