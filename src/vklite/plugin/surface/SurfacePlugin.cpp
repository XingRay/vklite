//
// Created by leixing on 2025/6/3.
//

#include "SurfacePlugin.h"

#include "vklite/Log.h"

namespace vklite {
    SurfacePlugin::SurfacePlugin() = default;

    SurfacePlugin::~SurfacePlugin() = default;

    std::vector<const char *> SurfacePlugin::getInstanceExtensions() {
        return {
            VK_KHR_SURFACE_EXTENSION_NAME
        };
    }

    std::vector<const char *> SurfacePlugin::getInstanceLayers() {
        return {
        };
    }

    void SurfacePlugin::onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) {
        LOG_D("instanceCreateInfo:%p", &instanceCreateInfo);
    }

    void SurfacePlugin::onInstanceCreated(Instance &instance) {
        LOG_D("instance:%p", &instance);
    }


    std::vector<const char *> SurfacePlugin::getDeviceExtensions() {
        return {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    }

    std::vector<const char *> SurfacePlugin::getDeviceLayers() {
        return {};
    }

    void SurfacePlugin::physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) {
        LOG_D("physicalDeviceFeatures:%p", &physicalDeviceFeatures);
    }

    void SurfacePlugin::onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {
        LOG_D("deviceCreateInfo:%p", &deviceCreateInfo);
    }

    /*
     *
     * static methods
     *
     */
    std::unique_ptr<SurfacePlugin> SurfacePlugin::buildUnique() {
        std::unique_ptr<SurfacePlugin> plugin = std::make_unique<SurfacePlugin>();

        return std::move(plugin);
    }
} // vklite
