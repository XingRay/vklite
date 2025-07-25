//
// Created by leixing on 2025/5/30.
//

#include "CombinedImageViewBuilder.h"
#include "vklite/Log.h"
#include "vklite/util/VulkanUtil.h"

namespace vklite {

    CombinedImageViewBuilder::CombinedImageViewBuilder()
            : mMemoryOffset(0), mMemoryPropertyFlags(vk::MemoryPropertyFlagBits::eDeviceLocal) {}

    CombinedImageViewBuilder::~CombinedImageViewBuilder() = default;

    CombinedImageViewBuilder &CombinedImageViewBuilder::device(vk::Device device) {
        mDevice = device;

        mImageBuilder.device(device);
        mDeviceMemoryBuilder.device(device);
        mImageViewBuilder.device(device);
        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::format(vk::Format format) {
        mImageBuilder.format(format);
        mImageViewBuilder.format(format);
        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::size(vk::Extent2D size) {
        mImageBuilder.size(size);
        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::size(uint32_t width, uint32_t height) {
        size(vk::Extent2D{width, height});
        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::sampleCount(vk::SampleCountFlagBits sampleCount) {
        mImageBuilder.sampleCount(sampleCount);
        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::memoryOffset(vk::DeviceSize memoryOffset) {
        mMemoryOffset = memoryOffset;
        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::memoryProperty(vk::MemoryPropertyFlags memoryProperty) {
        mMemoryPropertyFlags = memoryProperty;
        return *this;
    }

    CombinedImageView CombinedImageViewBuilder::build() {
        LOG_D("CombinedImageViewBuilder::build()");
        if (mDevice == nullptr) {
            throw std::runtime_error("CombinedImageViewBuilder::build(): mDevice == nullptr");
        }

        // create Image
        Image image = mImageBuilder.build();

        // create DeviceMemory
        vk::MemoryRequirements memoryRequirements = mDevice.getImageMemoryRequirements(image.getVkImage());
        mDeviceMemoryBuilder.allocationSize(memoryRequirements.size);

        uint32_t memoryTypeIndex = VulkanUtil::findMemoryTypeIndex(mPhysicalDeviceMemoryProperties, memoryRequirements, mMemoryPropertyFlags);
        mDeviceMemoryBuilder.memoryTypeIndex(memoryTypeIndex);

        DeviceMemory deviceMemory = mDeviceMemoryBuilder.build();

        // bind memory
        image.bindMemory(deviceMemory.getVkDeviceMemory(), mMemoryOffset);

        // create ImageView
        mImageViewBuilder.image(image.getVkImage());
        ImageView imageView = mImageViewBuilder.build();

        // combine Image / DeviceMemory / ImageView as CombinedMemoryImage
        return CombinedImageView{std::move(image), std::move(deviceMemory), std::move(imageView)};
    }

    std::vector<CombinedImageView> CombinedImageViewBuilder::build(uint32_t count) {
        std::vector<CombinedImageView> imageViews;
        imageViews.reserve(count);

        for (uint32_t i = 0; i < count; i++) {
            imageViews.emplace_back(build());
        }

        return imageViews;
    }

    std::unique_ptr<CombinedImageView> CombinedImageViewBuilder::buildUnique() {
        return std::make_unique<CombinedImageView>(build());
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::asDefault() {
        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::asColorAttachment() {
        asDefault();
        mImageBuilder.asColorAttachment();
        mImageViewBuilder.asColorAttachment();

        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::asDepthAttachment() {
        asDefault();
        mImageBuilder.asDepthAttachment();
        mImageViewBuilder.asDepthAttachment();

        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::asStorageImage() {
        asDefault();
        mImageBuilder.asStorageImage();
        mImageViewBuilder.asStorageImage();

        return *this;
    }

} // vklite