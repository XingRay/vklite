//
// Created by leixing on 2025/6/4.
//

#include "GraphicPipelineEngine.h"
#include "vklite/Log.h"

namespace vklite {
    GraphicPipelineEngine::GraphicPipelineEngine(
        uint32_t frameCount,
        vk::SampleCountFlagBits sampleCount,
        std::unique_ptr<Instance> instance,
        std::unique_ptr<Surface> surface,
        std::unique_ptr<PhysicalDevice> physicalDevice,
        std::unique_ptr<Device> device,
        std::unique_ptr<Queue> graphicQueue,
        std::unique_ptr<Queue> presentQueue,
        std::unique_ptr<Swapchain> swapchain,
        std::vector<ImageView> &&displayImageViews,
        std::vector<vk::Viewport> &&viewports,
        std::vector<vk::Rect2D> &&scissors,
        std::unique_ptr<CommandPool> commandPool,
        std::unique_ptr<CommandBuffers> commandBuffers,
        std::unique_ptr<RenderPass> renderPass,
        std::unique_ptr<CombinedImageView> colorImageView,
        std::unique_ptr<CombinedImageView> depthImageView,
        Framebuffers &&framebuffers,
        std::vector<Semaphore> &&imageAvailableSemaphores,
        std::vector<Semaphore> &&renderFinishedSemaphores,
        std::vector<Fence> &&fences,
        std::unique_ptr<PipelineLayout> pipelineLayout,
        std::unique_ptr<DescriptorPool> descriptorPool,
        DescriptorSetLayouts &&descriptorSetLayouts,
        std::vector<std::vector<vk::DescriptorSet> > &&descriptorSets,
        std::vector<PushConstant> &&pushConstants,
        std::unique_ptr<Pipeline> pipeline)
        : mFrameCount(frameCount),
          mSampleCount(sampleCount),
          mInstance(std::move(instance)),
          mSurface(std::move(surface)),
          mPhysicalDevice(std::move(physicalDevice)),
          mDevice(std::move(device)),
          mGraphicQueue(std::move(graphicQueue)),
          mPresentQueue(std::move(presentQueue)),
          mSwapchain(std::move(swapchain)),
          mDisplayImageViews(std::move(displayImageViews)),
          mViewports(std::move(viewports)),
          mScissors(std::move(scissors)),
          mCommandPool(std::move(commandPool)),
          mCommandBuffers(std::move(commandBuffers)),
          mRenderPass(std::move(renderPass)),
          mColorImageView(std::move(colorImageView)),
          mDepthImageView(std::move(depthImageView)),
          mFramebuffers(std::move(framebuffers)),
          mImageAvailableSemaphores(std::move(imageAvailableSemaphores)),
          mRenderFinishedSemaphores(std::move(renderFinishedSemaphores)),
          mFences(std::move(fences)),
          mPipelineLayout(std::move(pipelineLayout)),
          mDescriptorPool(std::move(descriptorPool)),
          mDescriptorSetLayouts(std::move(descriptorSetLayouts)),
          mDescriptorSets(std::move(descriptorSets)),
          mPushConstants(std::move(pushConstants)),
          mPipeline(std::move(pipeline)),
          mIndexCount(0) {
    }

    GraphicPipelineEngine::~GraphicPipelineEngine() = default;

    GraphicPipelineEngine::GraphicPipelineEngine(GraphicPipelineEngine &&other) noexcept
        : mFrameCount(other.mFrameCount),
          mSampleCount(other.mSampleCount),
          mInstance(std::move(other.mInstance)),
          mSurface(std::move(other.mSurface)),
          mPhysicalDevice(std::move(other.mPhysicalDevice)),
          mDevice(std::move(other.mDevice)),
          mGraphicQueue(std::move(other.mGraphicQueue)),
          mPresentQueue(std::move(other.mPresentQueue)),
          mSwapchain(std::move(other.mSwapchain)),
          mDisplayImageViews(std::move(other.mDisplayImageViews)),
          mViewports(std::move(other.mViewports)),
          mScissors(std::move(other.mScissors)),
          mCommandPool(std::move(other.mCommandPool)),
          mCommandBuffers(std::move(other.mCommandBuffers)),
          mRenderPass(std::move(other.mRenderPass)),
          mColorImageView(std::move(other.mColorImageView)),
          mDepthImageView(std::move(other.mDepthImageView)),
          mFramebuffers(std::move(other.mFramebuffers)),
          mImageAvailableSemaphores(std::move(other.mImageAvailableSemaphores)),
          mRenderFinishedSemaphores(std::move(other.mRenderFinishedSemaphores)),
          mFences(std::move(other.mFences)),
          mPipelineLayout(std::move(other.mPipelineLayout)),
          mDescriptorPool(std::move(other.mDescriptorPool)),
          mDescriptorSetLayouts(std::move(other.mDescriptorSetLayouts)),
          mDescriptorSets(std::move(other.mDescriptorSets)),
          mPushConstants(std::move(other.mPushConstants)),
          mPipeline(std::move(other.mPipeline)),
          mIndexCount(other.mIndexCount) {
    }

    GraphicPipelineEngine &GraphicPipelineEngine::operator=(GraphicPipelineEngine &&other) noexcept {
        if (this != &other) {
            mFrameCount = other.mFrameCount;
            mSampleCount = other.mSampleCount;
            // 移动赋值：逐个成员进行移动赋值
            mInstance = std::move(other.mInstance);
            mSurface = std::move(other.mSurface);
            mPhysicalDevice = std::move(other.mPhysicalDevice);
            mDevice = std::move(other.mDevice);
            mGraphicQueue = std::move(other.mGraphicQueue);
            mPresentQueue = std::move(other.mPresentQueue);
            mSwapchain = std::move(other.mSwapchain);
            mDisplayImageViews = std::move(other.mDisplayImageViews);
            mViewports = std::move(other.mViewports);
            mScissors = std::move(other.mScissors);
            mCommandPool = std::move(other.mCommandPool);
            mCommandBuffers = std::move(other.mCommandBuffers);
            mRenderPass = std::move(other.mRenderPass);
            mColorImageView = std::move(other.mColorImageView);
            mDepthImageView = std::move(other.mDepthImageView);
            mFramebuffers = std::move(other.mFramebuffers);
            mImageAvailableSemaphores = std::move(other.mImageAvailableSemaphores);
            mRenderFinishedSemaphores = std::move(other.mRenderFinishedSemaphores);
            mFences = std::move(other.mFences);
            mPipelineLayout = std::move(other.mPipelineLayout);
            mDescriptorPool = std::move(other.mDescriptorPool);
            mDescriptorSetLayouts = std::move(other.mDescriptorSetLayouts);
            mDescriptorSets = std::move(other.mDescriptorSets);
            mPushConstants = std::move(other.mPushConstants);
            mPipeline = std::move(other.mPipeline);
            mIndexCount = other.mIndexCount;
        }
        return *this;
    }

    Device &GraphicPipelineEngine::getDevice() const {
        return *mDevice;
    }

    PhysicalDevice &GraphicPipelineEngine::getPhysicalDevice() const {
        return *mPhysicalDevice;
    }

    CommandPool &GraphicPipelineEngine::getCommandPool() const {
        return *mCommandPool;
    }

    const std::vector<std::vector<vk::DescriptorSet> > &GraphicPipelineEngine::getDescriptorSets() const {
        return mDescriptorSets;
    }

    const vk::DescriptorSet &GraphicPipelineEngine::getDescriptorSets(uint32_t frameIndex, uint32_t set) const {
        return mDescriptorSets[frameIndex][set];
    }

    uint32_t GraphicPipelineEngine::getFrameIndex() const {
        return mCurrentFrameIndex;
    }

    uint32_t GraphicPipelineEngine::getFrameCount() const {
        return mFrameCount;
    }

    GraphicPipelineEngine &GraphicPipelineEngine::updateDescriptorSets(std::function<void(uint32_t, DescriptorSetMappingConfigure &)> &&configure) {
        DescriptorSetWriters descriptorSetWriters = DescriptorSetWritersBuilder()
                .frameCount(mFrameCount)
                .descriptorSetMappingConfigure(std::move(configure))
                .build();

        std::vector<vk::WriteDescriptorSet> writeDescriptorSets = descriptorSetWriters.createWriteDescriptorSets();

        bool debugLog = false;
        if (debugLog) {
            LOG_D("mDevice->getVkDevice().updateDescriptorSets: %zd", writeDescriptorSets.size());
            for (const vk::WriteDescriptorSet &writeDescriptorSet: writeDescriptorSets) {
                LOG_D("\twriteDescriptorSet:");
                LOG_D("\t\tdstSet:%p", (void *) writeDescriptorSet.dstSet);
                LOG_D("\t\tdstBinding:%d", writeDescriptorSet.dstBinding);
                LOG_D("\t\tdstArrayElement:%d", writeDescriptorSet.dstArrayElement);
                LOG_D("\t\tdescriptorType:%d", writeDescriptorSet.descriptorType);
                LOG_D("\t\tdescriptorCount:%d", writeDescriptorSet.descriptorCount);
                if (writeDescriptorSet.pBufferInfo != nullptr) {
                    for (uint32_t i = 0; i < writeDescriptorSet.descriptorCount; i++) {
                        const vk::DescriptorBufferInfo &bufferInfo = writeDescriptorSet.pBufferInfo[i];
                        LOG_D("\t\tbufferInfo:");
                        LOG_D("\t\t\tbuffer:%p", (void *) bufferInfo.buffer);
                        LOG_D("\t\t\toffset:%zd", bufferInfo.offset);
                        LOG_D("\t\t\trange:%zd", bufferInfo.range);
                    }
                }
                if (writeDescriptorSet.pImageInfo != nullptr) {
                    for (uint32_t i = 0; i < writeDescriptorSet.descriptorCount; i++) {
                        const vk::DescriptorImageInfo &imageInfo = writeDescriptorSet.pImageInfo[i];
                        LOG_D("\t\timageInfo:");
                        LOG_D("\t\t\timageView:%p", (void *) imageInfo.imageView);
                        LOG_D("\t\t\tsampler:%p", (void *) imageInfo.sampler);
                        LOG_D("\t\t\timageLayout:%p", (void *) imageInfo.imageLayout);
                    }
                }
            }
        }

        mDevice->getVkDevice().updateDescriptorSets(writeDescriptorSets, nullptr);

        return *this;
    }

    VertexBufferBuilder GraphicPipelineEngine::vertexBufferBuilder() {
        return VertexBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties());
    }

    GraphicPipelineEngine &GraphicPipelineEngine::addVertexBuffer(const vk::Buffer &buffer, vk::DeviceSize offset) {
        mVertexBuffers.push_back(buffer);
        mVertexBufferOffsets.push_back(offset);
        return *this;
    }

    GraphicPipelineEngine &GraphicPipelineEngine::addVertexBuffer(const VertexBuffer &buffer, vk::DeviceSize offset) {
        addVertexBuffer(buffer.getVkBuffer(), offset);
        return *this;
    }


    IndexBufferBuilder GraphicPipelineEngine::indexBufferBuilder() {
        return IndexBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties());
    }

    GraphicPipelineEngine &GraphicPipelineEngine::indexBuffer(const vk::Buffer &buffer, uint32_t indexCount) {
        mIndexBuffer = buffer;
        mIndexCount = indexCount;
        return *this;
    }

    GraphicPipelineEngine &GraphicPipelineEngine::indexBuffer(const IndexBuffer &buffer, uint32_t indexCount) {
        mIndexBuffer = buffer.getVkBuffer();
        mIndexCount = indexCount;
        return *this;
    }

    GraphicPipelineEngine &GraphicPipelineEngine::updatePushConstant(uint32_t index, const void *data, uint32_t size) {
        mPushConstants[index].update(data, size);
        return *this;
    }

    void GraphicPipelineEngine::drawIndexed() {
        Semaphore &imageAvailableSemaphore = mImageAvailableSemaphores[mCurrentFrameIndex];
        Semaphore &renderFinishedSemaphore = mRenderFinishedSemaphores[mCurrentFrameIndex];
        Fence &fence = mFences[mCurrentFrameIndex];

        vk::Result result = mFences[mCurrentFrameIndex].wait();
        if (result != vk::Result::eSuccess) {
            LOG_E("waitForFences failed");
            throw std::runtime_error("waitForFences failed");
        }

        // 当 acquireNextImageKHR 成功返回时，imageAvailableSemaphore 会被触发，表示图像已经准备好，可以用于渲染。
        auto [acquireResult, imageIndex] = mSwapchain->acquireNextImage(imageAvailableSemaphore.getVkSemaphore());
        if (acquireResult != vk::Result::eSuccess) {
            if (acquireResult == vk::Result::eErrorOutOfDateKHR) {
                // 交换链已与表面不兼容，不能再用于渲染。通常在窗口大小调整后发生。
                LOG_E("acquireNextImageKHR: eErrorOutOfDateKHR, recreateSwapChain");
                //                recreateSwapChain();
                return;
            } else if (acquireResult == vk::Result::eSuboptimalKHR) {
                //vk::Result::eSuboptimalKHR 交换链仍然可以成功显示到表面，但表面属性不再完全匹配。
                LOG_D("acquireNextImageKHR: eSuboptimalKHR");
            } else {
                LOG_E("acquireNextImageKHR: failed: %d", acquireResult);
                throw std::runtime_error("acquireNextImageKHR failed");
            }
        }

        const PooledCommandBuffer &commandBuffer = (*mCommandBuffers)[mCurrentFrameIndex];
        commandBuffer.record([&](const vk::CommandBuffer &vkCommandBuffer) {
            mRenderPass->execute(vkCommandBuffer, mFramebuffers[imageIndex].getVkFramebuffer(), [&](const vk::CommandBuffer &vkCommandBuffer) {
                vkCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline->getVkPipeline());
                vkCommandBuffer.setViewport(0, mViewports);
                vkCommandBuffer.setScissor(0, mScissors);

                if (!mDescriptorSets.empty()) {
                    vkCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mPipelineLayout->getVkPipelineLayout(), 0, mDescriptorSets[mCurrentFrameIndex], nullptr);
                }

                for (const PushConstant &pushConstant: mPushConstants) {
                    vkCommandBuffer.pushConstants(mPipelineLayout->getVkPipelineLayout(),
                                                  pushConstant.getStageFlags(),
                                                  pushConstant.getOffset(),
                                                  pushConstant.getSize(),
                                                  pushConstant.getData());
                }

                vkCommandBuffer.bindVertexBuffers(0, mVertexBuffers, mVertexBufferOffsets);
                vkCommandBuffer.bindIndexBuffer(mIndexBuffer, 0, vk::IndexType::eUint32);
                vkCommandBuffer.drawIndexed(mIndexCount, 1, 0, 0, 0);
            });
        });

        result = mFences[mCurrentFrameIndex].reset();
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("resetFences failed");
        }

        mGraphicQueue->submit(commandBuffer.getVkCommandBuffer(),
                              vk::PipelineStageFlagBits::eColorAttachmentOutput,
                              imageAvailableSemaphore.getVkSemaphore(),
                              renderFinishedSemaphore.getVkSemaphore(),
                              fence.getVkFence());

        result = mPresentQueue->present(mSwapchain->getVkSwapChain(), imageIndex, renderFinishedSemaphore.getVkSemaphore());
        if (result != vk::Result::eSuccess) {
            if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || mFramebufferResized) {
                mFramebufferResized = false;
                LOG_E("presentKHR: eErrorOutOfDateKHR or eSuboptimalKHR or mFramebufferResized, recreateSwapChain");
                // todo: recreateSwapChain
                //                recreateSwapChain();
                return;
            } else {
                throw std::runtime_error("presentKHR failed");
            }
        }

        mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mFrameCount;
    }

    UniformBufferBuilder GraphicPipelineEngine::uniformBufferBuilder() {
        return UniformBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties());
    }

    StorageBufferBuilder GraphicPipelineEngine::storageBufferBuilder() {
        return StorageBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties());
    }

    StagingBufferBuilder GraphicPipelineEngine::stagingBufferBuilder() {
        return StagingBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties());
    }

    CombinedImageSamplerBuilder GraphicPipelineEngine::samplerBuilder() {
        return CombinedImageSamplerBuilder().asDefault()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties());
    }
} // vklite
