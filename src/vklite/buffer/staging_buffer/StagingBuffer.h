//
// Created by leixing on 2025/3/6.
//

#pragma once

#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBuffer.h"

namespace vklite {

    class StagingBuffer {
    private:
        CombinedMemoryBuffer mCombinedMemoryBuffer;

    public:

        explicit StagingBuffer(CombinedMemoryBuffer &&combinedMemoryBuffer);

        ~StagingBuffer();

        StagingBuffer(const StagingBuffer &other) = delete;

        StagingBuffer &operator=(const StagingBuffer &other) = delete;

        StagingBuffer(StagingBuffer &&other) noexcept;

        StagingBuffer &operator=(StagingBuffer &&other) noexcept;

        [[nodiscard]]
        const CombinedMemoryBuffer &getCombinedMemoryBuffer() const;

        [[nodiscard]]
        const Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::Buffer &getVkBuffer() const;

        [[nodiscard]]
        vk::DeviceSize getSize() const;

        void updateBuffer(const void *data, vk::DeviceSize size, vk::DeviceSize offset, vk::MemoryMapFlags flags = vk::MemoryMapFlags{});

        void updateBuffer(const void *data, vk::DeviceSize size);
    };

} // vklite
