//
// Created by leixing on 2025/3/22.
//

#include "IndexBufferBuilder.h"
#include "vklite/buffer/index_buffer/IndexBufferMeta.h"
#include "vklite/Log.h"

namespace vklite {
    IndexBufferBuilder::IndexBufferBuilder()
        : mIndexType(vk::IndexType::eUint32) {
        mCombinedMemoryBufferBuilder.asDeviceLocal().addUsage(vk::BufferUsageFlagBits::eIndexBuffer);
    }

    IndexBufferBuilder::~IndexBufferBuilder() = default;

    IndexBufferBuilder &IndexBufferBuilder::device(vk::Device device) {
        mDevice = device;
        mCombinedMemoryBufferBuilder.device(device);
        return *this;
    }

    IndexBufferBuilder &IndexBufferBuilder::size(vk::DeviceSize size) {
        mCombinedMemoryBufferBuilder.size(size);
        return *this;
    }

    IndexBufferBuilder &IndexBufferBuilder::physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        mCombinedMemoryBufferBuilder.physicalDeviceMemoryProperties(physicalDeviceMemoryProperties);
        return *this;
    }

    IndexBufferBuilder &IndexBufferBuilder::indexType(vk::IndexType indexType) {
        mIndexType = indexType;
        return *this;
    }

    IndexBuffer IndexBufferBuilder::build() {
        LOG_D("IndexBufferBuilder::build()");
        if (mDevice == nullptr) {
            throw std::runtime_error("IndexBufferBuilder::build(): mDevice == nullptr");
        }
        if (!mPhysicalDeviceMemoryProperties.has_value()) {
            throw std::runtime_error("IndexBufferBuilder::build(): mPhysicalDeviceMemoryProperties not set");
        }

        return IndexBuffer{mDevice, mCombinedMemoryBufferBuilder.build(), mPhysicalDeviceMemoryProperties, IndexBufferMeta{mIndexType}};
    }

    std::unique_ptr<IndexBuffer> IndexBufferBuilder::buildUnique() {
        return std::make_unique<IndexBuffer>(build());
    }

    std::vector<IndexBuffer> IndexBufferBuilder::build(uint32_t count) {
        std::vector<IndexBuffer> buffers;
        buffers.reserve(count);

        for (uint32_t i = 0; i < count; i++) {
            buffers.emplace_back(mDevice, mCombinedMemoryBufferBuilder.build(), mPhysicalDeviceMemoryProperties, IndexBufferMeta{mIndexType});
        }

        return buffers;
    }
} // vklite
