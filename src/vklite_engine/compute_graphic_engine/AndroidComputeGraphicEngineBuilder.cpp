//
// Created by leixing on 2025/6/9.
//

#ifdef VKLITE_ANDROID

#include "AndroidComputeGraphicEngineBuilder.h"

#include "vklite/platform/android/surface/AndroidSurfaceBuilder.h"
#include "vklite/platform/android/plugin/AndroidSurfacePlugin.h"

namespace vklite {
    /**
     * preset
     */
    ComputeGraphicEngineBuilder AndroidComputeGraphicEngineBuilder::asDefault(ANativeWindow *window) {
        ComputeGraphicEngineBuilder computePipelineEngineBuilder;

        computePipelineEngineBuilder.asDefault()
                .addInstancePlugin(AndroidSurfacePlugin::buildUniqueCombined())
                .addDevicePlugin(AndroidSurfacePlugin::buildUniqueCombined())
                .surfaceBuilder([=](const vklite::Instance &instance) {
                    return AndroidSurfaceBuilder()
                            .instance(instance.getVkInstance())
                            .nativeWindow(window)
                            .buildUnique();
                });

        return std::move(computePipelineEngineBuilder);
    }
} // vklite
#endif
