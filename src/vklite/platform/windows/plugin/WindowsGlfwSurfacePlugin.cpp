//
// Created by leixing on 2025/6/3.
//
#ifdef VKLITE_WINDOWS
#include "WindowsGlfwSurfacePlugin.h"

#include <GLFW/glfw3.h>

#include "vklite/Log.h"

namespace vklite {
    WindowsGlfwSurfacePlugin::WindowsGlfwSurfacePlugin() = default;

    WindowsGlfwSurfacePlugin::~WindowsGlfwSurfacePlugin() = default;

    std::vector<const char *> WindowsGlfwSurfacePlugin::getInstanceExtensions() {
        uint32_t glfwRequiredExtensionCount = 0;
        // 查询 glfw 需要的 vulkan 扩展
        const char **glfwRequiredExtensions = glfwGetRequiredInstanceExtensions(&glfwRequiredExtensionCount);

        std::vector<const char *> extensions;
        extensions.reserve(glfwRequiredExtensionCount);

        for (uint32_t i = 0; i < glfwRequiredExtensionCount; i++) {
            extensions.push_back(glfwRequiredExtensions[i]);
        }

        return extensions;
    }

    std::vector<const char *> WindowsGlfwSurfacePlugin::getInstanceLayers() {
        return {
            "VK_LAYER_KHRONOS_validation"
        };
    }

    void WindowsGlfwSurfacePlugin::onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) {
    }

    void WindowsGlfwSurfacePlugin::onInstanceCreated(Instance &instance) {
    }


    std::vector<const char *> WindowsGlfwSurfacePlugin::getDeviceExtensions() {
        return {};
    }

    std::vector<const char *> WindowsGlfwSurfacePlugin::getDeviceLayers() {
        return {};
    }

    void WindowsGlfwSurfacePlugin::physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) {
    }

    void WindowsGlfwSurfacePlugin::onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {
    }

    /*
     *
     * static methods
     *
     */
    std::unique_ptr<WindowsGlfwSurfacePlugin> WindowsGlfwSurfacePlugin::buildUnique() {
        return std::make_unique<WindowsGlfwSurfacePlugin>();
    }
} // vklite
#endif
