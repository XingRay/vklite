//
// Created by leixing on 2025/4/28.
//
#ifdef VKLITE_WINDOWS

#include "WindowsPlugin.h"

#include "vklite/Log.h"

namespace vklite {

    std::vector<const char *> WindowsPlugin::getInstanceExtensions() {
        return {};
    }

    std::vector<const char *> WindowsPlugin::getDeviceExtensions() {
        return {};
    }

    std::vector<const char *> WindowsPlugin::getLayers() {
        return {};
    }

    void WindowsPlugin::onCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {
        LOG_D("deviceCreateInfo:%p", &deviceCreateInfo);
    }

} // vklite
#endif
