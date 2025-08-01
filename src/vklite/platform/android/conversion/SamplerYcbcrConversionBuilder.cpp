//
// Created by leixing on 2025/6/7.
//

#ifdef VKLITE_ANDROID

#include "SamplerYcbcrConversionBuilder.h"

#include "vklite/util/VkCheck.h"
#include "vklite/platform/android/api/VulkanAndroidApi.h"

namespace vklite {

    SamplerYcbcrConversionBuilder::SamplerYcbcrConversionBuilder() {
        mConversionCreateInfo
                .setPNext(&mExternalFormat)
                .setChromaFilter(vk::Filter::eNearest)
                .setForceExplicitReconstruction(false);
    };

    SamplerYcbcrConversionBuilder::~SamplerYcbcrConversionBuilder() = default;

    SamplerYcbcrConversionBuilder &SamplerYcbcrConversionBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    SamplerYcbcrConversionBuilder &SamplerYcbcrConversionBuilder::formatProperties(vk::AndroidHardwareBufferFormatPropertiesANDROID formatProperties) {
        LOG_D("SamplerYcbcrConversionBuilder::formatProperties()");
        // 确保设置了 formatProperties
        mFormatProperties = formatProperties;

        if (formatProperties.format == vk::Format::eUndefined) {
            mExternalFormat.externalFormat = formatProperties.externalFormat;
        }

        if (formatProperties.format == vk::Format::eUndefined) {
            mConversionCreateInfo.setYcbcrModel(formatProperties.suggestedYcbcrModel);
        } else {
            mConversionCreateInfo.setYcbcrModel(vk::SamplerYcbcrModelConversion::eYcbcr601);
        }

        mConversionCreateInfo
                .setFormat(formatProperties.format)
                .setYcbcrRange(formatProperties.suggestedYcbcrRange)
                .setComponents(formatProperties.samplerYcbcrConversionComponents)
                .setXChromaOffset(formatProperties.suggestedXChromaOffset)
                .setYChromaOffset(formatProperties.suggestedYChromaOffset);

        return *this;
    }

    SamplerYcbcrConversion SamplerYcbcrConversionBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("SamplerYcbcrConversionBuilder::build(): mDevice == nullptr");
        }
        if (!mFormatProperties.has_value()) {
            throw std::runtime_error("SamplerYcbcrConversionBuilder::build(): mFormatProperties not set");
        }

        vk::SamplerYcbcrConversion samplerYcbcrConversion;
        CALL_VK(vkCreateSamplerYcbcrConversion(mDevice, reinterpret_cast<VkSamplerYcbcrConversionCreateInfo *>(&mConversionCreateInfo), nullptr,
                                               reinterpret_cast<VkSamplerYcbcrConversion *>(&samplerYcbcrConversion)));
        return SamplerYcbcrConversion(mDevice, samplerYcbcrConversion);
    }

    std::unique_ptr<SamplerYcbcrConversion> SamplerYcbcrConversionBuilder::buildUnique() {
        return std::make_unique<SamplerYcbcrConversion>(build());
    }

} // vklite
#endif
