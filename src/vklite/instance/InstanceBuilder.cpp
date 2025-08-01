//
// Created by leixing on 2025/4/27.
//

#ifndef VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#endif

#include <vulkan/vulkan.hpp>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

#include "InstanceBuilder.h"

#include "vklite/util/selector/cstring/CStringFixListSelector.h"
#include "vklite/util/selector/cstring/CStringLambdaListSelector.h"
#include "vklite/util/selector/cstring/CStringRequiredAndOptionalListSelector.h"
#include "vklite/util/VkCheck.h"
#include "vklite/util/VkCheckCpp.h"
#include "vklite/Log.h"
#include "vklite/util/StringUtil.h"
#include "vklite/instance/InstanceApi.h"
#include "vklite/util/CStringUtil.h"
#include "vklite/util/VulkanUtil.h"

namespace vklite {
    InstanceBuilder::InstanceBuilder() {
        mApplicationInfo = vk::ApplicationInfo{};
        mApplicationInfo
                .setPApplicationName(VKLITE_APPLICATION_NAME)
                .setApiVersion(VKLITE_APPLICATION_VERSION)
                .setPEngineName(VKLITE_ENGINE_NAME)
                .setEngineVersion(VKLITE_ENGINE_VERSION)
                .setApiVersion(VK_API_VERSION_1_3);

        mInstanceCreateInfo = vk::InstanceCreateInfo{};
        mInstanceCreateInfo
                .setPApplicationInfo(&mApplicationInfo)
                .setPEnabledLayerNames(mEnableLayers)
                .setPEnabledExtensionNames(mExtensions);
    }

    InstanceBuilder::~InstanceBuilder() = default;

    InstanceBuilder::InstanceBuilder(InstanceBuilder &&other) noexcept
        : mInstanceCreateInfo(other.mInstanceCreateInfo),
          mApplicationInfo(other.mApplicationInfo),
          mExtensions(std::move(other.mExtensions)),
          mEnableLayers(std::move(other.mEnableLayers)),
          mPlugins(std::move(other.mPlugins)) {
        mInstanceCreateInfo.setPApplicationInfo(&mApplicationInfo);
    }

    InstanceBuilder &InstanceBuilder::operator=(InstanceBuilder &&other) noexcept {
        if (this != &other) {
            mInstanceCreateInfo = other.mInstanceCreateInfo;
            mApplicationInfo = other.mApplicationInfo;
            mExtensions = std::move(other.mExtensions);
            mEnableLayers = std::move(other.mEnableLayers);
            mPlugins = std::move(other.mPlugins);

            mInstanceCreateInfo.setPApplicationInfo(&mApplicationInfo);
        }
        return *this;
    }

    InstanceBuilder &InstanceBuilder::applicationName(const char *applicationName) {
        mApplicationInfo.setPApplicationName(applicationName);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::applicationVersion(uint32_t applicationVersion) {
        mApplicationInfo.setApplicationVersion(applicationVersion);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::engineName(const char *engineName) {
        mApplicationInfo.setPEngineName(engineName);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::engineVersion(uint32_t engineVersion) {
        mApplicationInfo.setEngineVersion(engineVersion);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::extensions(std::vector<const char *> &&extensions) {
        mExtensions = std::move(extensions);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::layers(std::vector<const char *> &&layers) {
        mEnableLayers = std::move(layers);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::addPlugin(std::unique_ptr<PluginInterface> plugin) {
        mPlugins.push_back(std::move(plugin));
        return *this;
    }

    Instance InstanceBuilder::build() {
        LOG_D("Instance::Instance");
        LOADER_NAMESPACE::DynamicLoader dynamicLoader;
        InstanceApi::initInstanceApi(dynamicLoader);

        // InstanceExtension
        std::vector<vk::ExtensionProperties> instanceExtensionProperties = InstanceApi::enumerateInstanceExtensionProperties();
        VulkanUtil::printExtensions(instanceExtensionProperties);
        //        LOG_D("Available instance extensions:[%ld]", instanceExtensionProperties.size());
        //        for (const vk::ExtensionProperties &extensionProperty: instanceExtensionProperties) {
        //            LOG_D("\t%s ( version:%d )", extensionProperty.extensionName.data(), extensionProperty.specVersion);
        //        }

        for (const std::unique_ptr<PluginInterface> &plugin: mPlugins) {
            std::vector<const char *> pluginExtensions = plugin->getInstanceExtensions();
            mExtensions.insert(mExtensions.begin(), std::move_iterator(pluginExtensions.begin()), std::move_iterator(pluginExtensions.end()));
        }
        mExtensions = CStringUtil::removeDuplicates(mExtensions);
        LOG_D("enabled instance extensions:[%zd]", mExtensions.size());
        for (const auto &extensionName: mExtensions) {
            LOG_D("  extensionName: %s", extensionName);
        }
        mInstanceCreateInfo.setPEnabledExtensionNames(mExtensions);

        // Available Layers
        std::vector<std::string> availableLayerNames = InstanceApi::enumerateInstanceLayerNames();
        LOG_D("Available Layers : [%zd]", availableLayerNames.size());
        for (const auto &name: availableLayerNames) {
            LOG_D("  LayerName:  %s", name.c_str());
        }

        // plugin->getInstanceLayers
        LOG_D("mPlugins.size:%zd", mPlugins.size());
        for (const std::unique_ptr<PluginInterface> &plugin: mPlugins) {
            std::vector<const char *> pluginLayers = plugin->getInstanceLayers();
            mEnableLayers.insert(mEnableLayers.begin(), std::move_iterator(pluginLayers.begin()), std::move_iterator(pluginLayers.end()));
        }
        mEnableLayers = CStringUtil::removeDuplicates(mEnableLayers);
        LOG_D("enabled layer names:[%zd]", mEnableLayers.size());
        for (const auto &layerName: mEnableLayers) {
            LOG_D("  enabled layer name: %s", layerName);
        }
        mInstanceCreateInfo.setPEnabledLayerNames(mEnableLayers);


        // plugin->onPreCreateInstance
        for (const std::unique_ptr<PluginInterface> &plugin: mPlugins) {
            plugin->onPreCreateInstance((vk::InstanceCreateInfo &) mInstanceCreateInfo);
        }

        LOG_D("InstanceApi::createInstance");
        vk::Instance vkInstance = InstanceApi::createInstance(mInstanceCreateInfo);
        LOG_D("InstanceApi::createInstance: %p", static_cast<void*>(vkInstance));

        LOADER_NAMESPACE::DispatchLoaderDynamic dispatchLoaderDynamic(vkInstance, vkGetInstanceProcAddr);
        dispatchLoaderDynamic.init(dynamicLoader);
        LOADER_NAMESPACE::defaultDispatchLoaderDynamic = dispatchLoaderDynamic;

        Instance instance(vkInstance);

        for (const std::unique_ptr<PluginInterface> &plugin: mPlugins) {
            plugin->onInstanceCreated(instance);
        }

        return instance;
    }

    std::unique_ptr<Instance> InstanceBuilder::buildUnique() {
        return std::make_unique<Instance>(build());
    }
} // vklite
