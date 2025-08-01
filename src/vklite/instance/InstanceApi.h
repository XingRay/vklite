//
// Created by leixing on 2025/5/28.
//

#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

#ifdef VKLITE_ANDROID

#define LOADER_NAMESPACE vk

#else

// #define LOADER_NAMESPACE vk::detail
#define LOADER_NAMESPACE vk

#endif


namespace vklite {
    extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
    extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
    extern PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
    extern PFN_vkCreateInstance vkCreateInstance;

    class InstanceApi {
    public:
        static void initInstanceApi(const LOADER_NAMESPACE::DynamicLoader &dynamicLoader);

        static std::vector<vk::ExtensionProperties> enumerateInstanceExtensionProperties();

        static std::vector<std::string> enumerateInstanceExtensionNames();

        static std::vector<vk::LayerProperties> enumerateInstanceLayerProperties();

        static std::vector<std::string> enumerateInstanceLayerNames();

        static vk::Instance createInstance(const vk::InstanceCreateInfo &instanceCreateInfo);
    };
} // vklite
