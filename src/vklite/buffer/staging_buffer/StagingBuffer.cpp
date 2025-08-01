//
// Created by leixing on 2025/3/6.
//

#include "StagingBuffer.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

    StagingBuffer::StagingBuffer(CombinedMemoryBuffer &&combinedMemoryBuffer)
            : mCombinedMemoryBuffer(std::move(combinedMemoryBuffer)) {}

    StagingBuffer::~StagingBuffer() = default;

    StagingBuffer::StagingBuffer(StagingBuffer &&other) noexcept
            : mCombinedMemoryBuffer(std::move(other.mCombinedMemoryBuffer)) {}

    StagingBuffer &StagingBuffer::operator=(StagingBuffer &&other) noexcept {
        if (this != &other) {
            mCombinedMemoryBuffer = std::move(other.mCombinedMemoryBuffer);
        }
        return *this;
    }

    const CombinedMemoryBuffer &StagingBuffer::getCombinedMemoryBuffer() const{
        return mCombinedMemoryBuffer;
    }

    const Buffer &StagingBuffer::getBuffer() const {
        return mCombinedMemoryBuffer.getBuffer();
    }

    const vk::Buffer &StagingBuffer::getVkBuffer() const {
        return mCombinedMemoryBuffer.getVkBuffer();
    }

    vk::DeviceSize StagingBuffer::getSize() const {
        return mCombinedMemoryBuffer.getBuffer().getSize();
    }

    void StagingBuffer::updateBuffer(const void *data, vk::DeviceSize size, vk::DeviceSize offset, vk::MemoryMapFlags flags) {
        mCombinedMemoryBuffer.getDeviceMemory().updateBuffer(data, size, offset, flags);
    }

    void StagingBuffer::updateBuffer(const void *data, vk::DeviceSize size) {
        updateBuffer(data, size, 0);
    }

} // vklite