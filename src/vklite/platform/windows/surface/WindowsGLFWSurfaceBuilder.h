//
// Created by leixing on 2025/1/2.
//

#pragma once

#ifdef VKLITE_WINDOWS
#include <functional>

// That way GLFW will include its own definitions and automatically load the Vulkan header with it.
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.hpp>

#include "vklite/surface/Surface.h"
#include "vklite/surface/SurfaceBuilder.h"

namespace vklite {
    class WindowsGLFWSurfaceBuilder : public SurfaceBuilder {
    private:
        vk::Instance mInstance;
        GLFWwindow *mWindow;

    public:
        WindowsGLFWSurfaceBuilder();

        WindowsGLFWSurfaceBuilder(const WindowsGLFWSurfaceBuilder &other) = delete;

        WindowsGLFWSurfaceBuilder &operator=(const WindowsGLFWSurfaceBuilder &other) = delete;

        WindowsGLFWSurfaceBuilder(WindowsGLFWSurfaceBuilder &&other) noexcept;

        WindowsGLFWSurfaceBuilder &operator=(WindowsGLFWSurfaceBuilder &&other) noexcept;

        ~WindowsGLFWSurfaceBuilder() override;

        WindowsGLFWSurfaceBuilder &instance(vk::Instance instance);

        WindowsGLFWSurfaceBuilder &window(GLFWwindow *window);

        [[nodiscard]]
        Surface build() const override;
    };
}


#endif
