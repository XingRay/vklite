//
// Created by leixing on 2025-07-24.
//

#include "ValidationPlugin.h"

namespace vklite {
    ValidationPlugin::ValidationPlugin() = default;

    ValidationPlugin::~ValidationPlugin() = default;

    std::vector<const char *> ValidationPlugin::getInstanceExtensions() {
        return {};
    }

    std::vector<const char *> ValidationPlugin::getInstanceLayers() {
        return {
            "VK_LAYER_KHRONOS_validation"
        };
    }

    void ValidationPlugin::onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) {
    }

    void ValidationPlugin::onInstanceCreated(Instance &instance) {
    }


    std::vector<const char *> ValidationPlugin::getDeviceExtensions() {
        return {};
    }

    std::vector<const char *> ValidationPlugin::getDeviceLayers() {
        return {};
    }

    void ValidationPlugin::physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) {
    }

    void ValidationPlugin::onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {
    }

    /*
     *
     * static methods
     *
     */
    std::unique_ptr<ValidationPlugin> ValidationPlugin::buildUnique() {
        return std::make_unique<ValidationPlugin>();
    }
} // vklite
