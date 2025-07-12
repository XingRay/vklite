//
// Created by leixing on 2025/1/2.
//

#ifdef VKLITE_WINDOWS

#include "vklite/platform/windows/surface/WindowsGLFWSurfaceBuilder.h"
#include "vklite/Log.h"
#include "vklite/surface/Surface.h"

namespace vklite {
    WindowsGLFWSurfaceBuilder::WindowsGLFWSurfaceBuilder() = default;

    WindowsGLFWSurfaceBuilder::WindowsGLFWSurfaceBuilder(WindowsGLFWSurfaceBuilder &&other) noexcept
        : mInstance(std::exchange(other.mInstance, nullptr)),
          mWindow(std::exchange(other.mWindow, nullptr)) {
    }

    WindowsGLFWSurfaceBuilder &WindowsGLFWSurfaceBuilder::operator=(WindowsGLFWSurfaceBuilder &&other) noexcept {
        if (this != &other) {
            mInstance = std::exchange(other.mInstance, nullptr);
            mWindow = std::exchange(other.mWindow, nullptr);
        }
        return *this;
    }

    WindowsGLFWSurfaceBuilder::~WindowsGLFWSurfaceBuilder() = default;

    WindowsGLFWSurfaceBuilder &WindowsGLFWSurfaceBuilder::instance(vk::Instance instance) {
        mInstance = instance;
        return *this;
    }

    WindowsGLFWSurfaceBuilder &WindowsGLFWSurfaceBuilder::window(GLFWwindow *window) {
        mWindow = window;
        return *this;
    }

    [[nodiscard]]
    Surface WindowsGLFWSurfaceBuilder::build() const {
        LOG_D("WindowsGLFWSurfaceBuilder::build()");
        vk::SurfaceKHR surface;
        VkResult result = glfwCreateWindowSurface(mInstance, mWindow, nullptr, reinterpret_cast<VkSurfaceKHR *>(&surface));
        if (result != VK_SUCCESS) {
            LOG_E("failed to create surface on windows !");
            throw std::runtime_error("failed to create surface on windows !");
        }
        return Surface(mInstance, surface);
    };
}

#endif
