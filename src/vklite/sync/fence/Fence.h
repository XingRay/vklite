//
// Created by leixing on 2025/5/28.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {
    /**
     * fence主要用于CPU和GPU之间的同步
     */
    class Fence {
    private:
        vk::Device mDevice;
        vk::Fence mFence;

    public:
        explicit Fence(vk::Device device, vk::Fence fence);

        ~Fence();

        Fence(const Fence& other) = delete;

        Fence& operator=(const Fence& other) = delete;

        Fence(Fence&& other) noexcept;

        Fence& operator=(Fence&& other) noexcept;

        [[nodiscard]]
        const vk::Fence& getVkFence() const;

        [[nodiscard]]
        vk::Result reset();

        [[nodiscard]]
        vk::Result wait(uint64_t timeout);

        [[nodiscard]]
        vk::Result wait();

        [[nodiscard]]
        vk::Result waitAndReset(uint64_t timeout);

        [[nodiscard]]
        vk::Result waitAndReset();
    };
} // vklite
