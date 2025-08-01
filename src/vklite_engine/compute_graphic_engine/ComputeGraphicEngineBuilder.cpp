//
// Created by leixing on 2025/6/9.
//

#include "ComputeGraphicEngineBuilder.h"

namespace vklite {
    ComputeGraphicEngineBuilder::ComputeGraphicEngineBuilder()
        : mInstanceBuilder{}, mDeviceBuilder{} {
    }

    ComputeGraphicEngineBuilder::~ComputeGraphicEngineBuilder() = default;

    ComputeGraphicEngineBuilder::ComputeGraphicEngineBuilder(ComputeGraphicEngineBuilder &&other) noexcept
        : mFrameCount(std::exchange(other.mFrameCount, 0)),
          mClearColor(other.mClearColor),
          mClearDepth(std::exchange(other.mClearDepth, 0.0f)),
          mMsaaEnable(std::exchange(other.mMsaaEnable, false)),
          mDepthTestEnable(std::exchange(other.mDepthTestEnable, false)),

          mGraphicShaderConfigure(std::move(other.mGraphicShaderConfigure)),
          mInstanceBuilder(std::move(other.mInstanceBuilder)),
          mSurfaceBuilder(std::move(other.mSurfaceBuilder)),
          mPhysicalDeviceSelector(std::move(other.mPhysicalDeviceSelector)),
          mSampleCountSelector(std::move(other.mSampleCountSelector)),
          mDeviceBuilder(std::move(other.mDeviceBuilder)) {
    }

    ComputeGraphicEngineBuilder &ComputeGraphicEngineBuilder::operator=(ComputeGraphicEngineBuilder &&other) noexcept {
        if (this != &other) {
            // 基本类型：直接赋值（移动等同于拷贝）
            mFrameCount = other.mFrameCount;
            mClearColor = other.mClearColor;
            mClearDepth = other.mClearDepth;
            mMsaaEnable = other.mMsaaEnable;
            mDepthTestEnable = other.mDepthTestEnable;

            // 自定义类型：使用移动赋值
            mGraphicShaderConfigure = std::move(other.mGraphicShaderConfigure);
            mInstanceBuilder = std::move(other.mInstanceBuilder);
            mSurfaceBuilder = std::move(other.mSurfaceBuilder);
            mPhysicalDeviceSelector = std::move(other.mPhysicalDeviceSelector);
            mSampleCountSelector = std::move(other.mSampleCountSelector);
            mDeviceBuilder = std::move(other.mDeviceBuilder);
        }
        return *this;
    }

    ComputeGraphicEngineBuilder &ComputeGraphicEngineBuilder::frameCount(uint32_t frameCount) {
        mFrameCount = frameCount;
        return *this;
    }

    ComputeGraphicEngineBuilder &ComputeGraphicEngineBuilder::clearColor(const std::array<float, 4> &clearColor) {
        mClearColor = clearColor;
        return *this;
    }

    ComputeGraphicEngineBuilder &ComputeGraphicEngineBuilder::clearColor(float r, float g, float b, float a) {
        clearColor(std::array<float, 4>{r, g, b, a});
        return *this;
    }

    ComputeGraphicEngineBuilder &ComputeGraphicEngineBuilder::clearColor(float r, float g, float b) {
        clearColor(r, g, b, 1.0f);
        return *this;
    }

    ComputeGraphicEngineBuilder &ComputeGraphicEngineBuilder::clearDepth(float clearDepth) {
        mClearDepth = clearDepth;
        return *this;
    }

    ComputeGraphicEngineBuilder &ComputeGraphicEngineBuilder::addInstancePlugin(std::unique_ptr<PluginInterface> plugin) {
        mInstanceBuilder.addPlugin(std::move(plugin));
        return *this;
    }

    ComputeGraphicEngineBuilder &ComputeGraphicEngineBuilder::addDevicePlugin(std::unique_ptr<PluginInterface> plugin) {
        mDeviceBuilder.addPlugin(std::move(plugin));
        return *this;
    }

    ComputeGraphicEngineBuilder &ComputeGraphicEngineBuilder::surfaceBuilder(std::function<std::unique_ptr<Surface>(const Instance &Instance)> &&surfaceBuilder) {
        mSurfaceBuilder = std::move(surfaceBuilder);
        return *this;
    }

    ComputeGraphicEngineBuilder &ComputeGraphicEngineBuilder::physicalDeviceSelector(
        std::function<std::unique_ptr<PhysicalDevice>(const Instance &Instance, const Surface &surface)> &&physicalDeviceSelector) {
        mPhysicalDeviceSelector = std::move(physicalDeviceSelector);
        return *this;
    }

    ComputeGraphicEngineBuilder &ComputeGraphicEngineBuilder::sampleCountSelector(
        std::function<vk::SampleCountFlagBits(const std::vector<vk::SampleCountFlagBits> &sampleCountFlagBits)> &&sampleCountSelector) {
        mSampleCountSelector = std::move(sampleCountSelector);
        return *this;
    }

    ComputeGraphicEngineBuilder &ComputeGraphicEngineBuilder::enableDepthTest() {
        mDepthTestEnable = true;
        return *this;
    }

    ComputeGraphicEngineBuilder &ComputeGraphicEngineBuilder::graphicShaderConfigure(ShaderConfigure &&shaderConfigure) {
        mGraphicShaderConfigure = std::move(shaderConfigure);
        return *this;
    }

    ComputeGraphicEngineBuilder &ComputeGraphicEngineBuilder::computeShaderConfigure(ShaderConfigure &&shaderConfigure) {
        mComputeShaderConfigure = std::move(shaderConfigure);
        return *this;
    }

    ComputeGraphicEngine ComputeGraphicEngineBuilder::build() {
        std::unique_ptr<Instance> instance = mInstanceBuilder.buildUnique();
        std::unique_ptr<Surface> surface = mSurfaceBuilder(*instance);
        std::unique_ptr<PhysicalDevice> physicalDevice = mPhysicalDeviceSelector(*instance, *surface);

        vk::SampleCountFlagBits sampleCount = vk::SampleCountFlagBits::e1;
        if (mSampleCountSelector != nullptr) {
            sampleCount = mSampleCountSelector(physicalDevice->querySampleCountFlagBits());
        }
        if (sampleCount != vk::SampleCountFlagBits::e1) {
            mMsaaEnable = true;
        }

        uint32_t presentQueueFamilyIndex = physicalDevice->queryQueueFamilyIndicesBySurface(surface->getVkSurface())[0];
        uint32_t computeAndGraphicQueueFamilyIndex = physicalDevice->queryQueueFamilyIndicesByFlags(vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute)[0];

        std::unique_ptr<Device> device = mDeviceBuilder
                .physicalDevice(physicalDevice->getVkPhysicalDevice())
                .addQueueFamily(computeAndGraphicQueueFamilyIndex)
                .addQueueFamily(presentQueueFamilyIndex)
                .buildUnique();
        LOG_D("device => %p", (void *) device->getVkDevice());

        std::unique_ptr<Queue> graphicQueue = std::make_unique<Queue>(device->getQueue(computeAndGraphicQueueFamilyIndex));
        std::unique_ptr<Queue> presentQueue = std::make_unique<Queue>(device->getQueue(presentQueueFamilyIndex));

        std::unique_ptr<Swapchain> swapchain = SwapchainBuilder()
                .device(device->getVkDevice())
                .surface(surface->getVkSurface())
                .queueFamilyIndices({presentQueueFamilyIndex})
                .config(physicalDevice->getVkPhysicalDevice(), surface->getVkSurface())
                .buildUnique();

        std::vector<vk::Viewport> viewports = swapchain->fullScreenViewports();
        std::vector<vk::Rect2D> scissors = swapchain->fullScreenScissors();

        std::unique_ptr<CommandPool> commandPool = CommandPoolBuilder()
                .device(device->getVkDevice())
                .queueFamilyIndex(computeAndGraphicQueueFamilyIndex)
                .buildUnique();
        std::unique_ptr<CommandBuffers> commandBuffers = commandPool->allocateUnique(mFrameCount);

        // 创建附件
        std::vector<ImageView> displayImageViews = swapchain->createDisplayImageViews();

        std::unique_ptr<CombinedImageView> colorImageView = nullptr;
        if (mMsaaEnable) {
            colorImageView = CombinedImageViewBuilder().asColorAttachment()
                    .device(device->getVkDevice())
                    .format(swapchain->getVkFormat())
                    .size(swapchain->getDisplaySize())
                    .sampleCount(sampleCount)
                    .physicalDeviceMemoryProperties(physicalDevice->getMemoryProperties())
                    .buildUnique();
        }

        std::unique_ptr<CombinedImageView> depthImageView = nullptr;
        if (mDepthTestEnable) {
            depthImageView = CombinedImageViewBuilder().asDepthAttachment()
                    .device(device->getVkDevice())
                    .format(physicalDevice->findDepthFormat())
                    .size(swapchain->getDisplaySize())
                    .sampleCount(sampleCount)
                    .physicalDeviceMemoryProperties(physicalDevice->getMemoryProperties())
                    .buildUnique();
        }

        Subpass externalSubpass = Subpass::externalSubpass();
        std::unique_ptr<RenderPass> renderPass = RenderPassBuilder()
                .device(device->getVkDevice())
                .renderAreaExtend(swapchain->getDisplaySize())
                .addSubpass([&](Subpass &subpass, [[maybe_unused]] const std::vector<Subpass> &subpasses) {
                    subpass
                            .pipelineBindPoint(vk::PipelineBindPoint::eGraphics)
                            .addDependency(externalSubpass,
                                           vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                                           vk::AccessFlagBits::eNone,
                                           vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                                           vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite);
                })
                .addAttachmentIf(mMsaaEnable, [&](Attachment &attachment, std::vector<Subpass> &subpasses) {
                    attachment.msaaColorAttachment()
                            .sampleCount(sampleCount)
                            .format(swapchain->getVkFormat())
                            .clearColorValue(mClearColor)
                            .asColorAttachmentUsedIn(subpasses[0], vk::ImageLayout::eColorAttachmentOptimal);
                })
                .addAttachment([&](Attachment &attachment, std::vector<Subpass> &subpasses) {
                    attachment.presentColorAttachment()
                            .format(swapchain->getVkFormat())
                            .clearColorValue(mClearColor)
                            .applyIf(mMsaaEnable, [&](Attachment &thiz) {
                                thiz
                                        .loadOp(vk::AttachmentLoadOp::eDontCare)
                                        .asResolveAttachmentUsedIn(subpasses[0], vk::ImageLayout::eColorAttachmentOptimal);
                            })
                            .applyIf(!mMsaaEnable, [&](Attachment &thiz) {
                                thiz.asColorAttachmentUsedIn(subpasses[0], vk::ImageLayout::eColorAttachmentOptimal);
                            });
                })
                .addAttachmentIf(mDepthTestEnable, [&](Attachment &attachment, std::vector<Subpass> &subpasses) {
                    attachment.depthStencilAttachment()
                            .sampleCount(sampleCount)
                            .clearDepthValue(mClearDepth)
                            .format(physicalDevice->findDepthFormat())
                            .asDepthStencilAttachmentUsedIn(subpasses[0], vk::ImageLayout::eDepthStencilAttachmentOptimal);
                })
                .buildUnique();

        Framebuffers framebuffers = FramebuffersBuilder()
                .count(static_cast<uint32_t>(displayImageViews.size()))
                .framebufferBuilder([&](uint32_t index) {
                    return FramebufferBuilder()
                            .device(device->getVkDevice())
                            .renderPass(renderPass->getVkRenderPass())
                            .width(swapchain->getDisplaySize().width)
                            .height(swapchain->getDisplaySize().height)
                            // 下面添加附件的顺序不能乱, 附件的顺序由 RenderPass 的附件定义顺序决定，必须严格一致。
                            .addAttachmentIf(mMsaaEnable, [&]() { return colorImageView->getVkImageView(); })
                            .addAttachment(displayImageViews[index].getVkImageView())
                            .addAttachmentIf(mDepthTestEnable, [&]() { return depthImageView->getVkImageView(); })
                            .build();
                })
                .build();

        std::vector<Semaphore> imageAvailableSemaphores = SemaphoreBuilder().device(device->getVkDevice()).build(mFrameCount);
        std::vector<Semaphore> renderFinishedSemaphores = SemaphoreBuilder().device(device->getVkDevice()).build(mFrameCount);
        std::vector<Fence> fences = FenceBuilder()
                .device(device->getVkDevice())
                // 已发出信号的状态下创建栅栏，以便第一次调用 vkWaitForFences()立即返回
                .fenceCreateFlags(vk::FenceCreateFlagBits::eSignaled)
                .build(mFrameCount);

        std::vector<vk::PushConstantRange> graphicPushConstantRanges = mGraphicShaderConfigure.getPushConstantRanges();
        std::vector<PushConstant> graphicPushConstants;
        graphicPushConstants.reserve(graphicPushConstantRanges.size());
        for (const vk::PushConstantRange &pushConstantRange: graphicPushConstantRanges) {
            graphicPushConstants.emplace_back(pushConstantRange.size, pushConstantRange.offset, pushConstantRange.stageFlags);
        }

        std::unique_ptr<DescriptorPool> descriptorPool = vklite::DescriptorPoolBuilder()
                .device(device->getVkDevice())
                .frameCount(mFrameCount)
                .addDescriptorPoolSizes(mComputeShaderConfigure.calcDescriptorPoolSizes())
                .addDescriptorSetCount(mComputeShaderConfigure.getDescriptorSetCount())
                .addDescriptorPoolSizes(mGraphicShaderConfigure.calcDescriptorPoolSizes())
                .addDescriptorSetCount(mGraphicShaderConfigure.getDescriptorSetCount())
                .buildUnique();

        std::unique_ptr<CombinedPipeline> graphicPipeline = CombinedGraphicPipelineBuilder()
                .device(device->getVkDevice())
                .frameCount(mFrameCount)
                .renderPass(renderPass->getVkRenderPass())
                .shaderConfigure(mGraphicShaderConfigure)
                .topology(vk::PrimitiveTopology::ePointList)
                .viewports(viewports)
                .scissors(scissors)
                .sampleCount(sampleCount)
                .depthTestEnable(mDepthTestEnable)
                .buildUnique();


        std::unique_ptr<Queue> computeQueue = std::make_unique<Queue>(device->getQueue(computeAndGraphicQueueFamilyIndex));
        std::unique_ptr<CommandBuffers> computeCommandBuffers = commandPool->allocateUnique(mFrameCount);

        std::unique_ptr<CombinedPipeline> computePipeline = vklite::CombinedComputePipelineBuilder()
                .device(device->getVkDevice())
                .frameCount(mFrameCount)
                .descriptorPool(descriptorPool->getVkDescriptorPool())
                .shaderConfigure(mComputeShaderConfigure)
                .buildUnique();

        std::vector<Fence> computeFences = vklite::FenceBuilder()
                .device(device->getVkDevice())
                // 已发出信号的状态下创建栅栏，以便第一次调用 vkWaitForFences()立即返回
                .fenceCreateFlags(vk::FenceCreateFlagBits::eSignaled)
                .build(mFrameCount);

        std::vector<Semaphore> computeFinishSemaphores = vklite::SemaphoreBuilder()
                .device(device->getVkDevice())
                .build(mFrameCount);


        return ComputeGraphicEngine{
            mFrameCount,
            sampleCount,

            std::move(instance),
            std::move(surface),
            std::move(physicalDevice),
            std::move(device),
            std::move(graphicQueue),
            std::move(presentQueue),
            std::move(swapchain),
            std::move(displayImageViews),
            std::move(viewports),
            std::move(scissors),
            std::move(commandPool),
            std::move(commandBuffers),
            std::move(renderPass),
            std::move(colorImageView),
            std::move(depthImageView),
            std::move(framebuffers),
            std::move(imageAvailableSemaphores),
            std::move(renderFinishedSemaphores),
            std::move(fences),

            std::move(descriptorPool),
            std::move(graphicPipeline),

            std::move(computeQueue),
            std::move(computeCommandBuffers),
            std::move(computeFences),
            std::move(computeFinishSemaphores),

            std::move(computePipeline)
        };
    }

    std::unique_ptr<ComputeGraphicEngine> ComputeGraphicEngineBuilder::buildUnique() {
        return std::make_unique<ComputeGraphicEngine>(build());
    }


    ComputeGraphicEngineBuilder &ComputeGraphicEngineBuilder::asDefault() {
        (*this).physicalDeviceSelector([](const vklite::Instance &instance, const vklite::Surface &surface) {
                    return vklite::PhysicalDeviceSelector::makeDefault(surface)
                            .selectUnique(instance.enumeratePhysicalDevices());
                })
                .sampleCountSelector([](const std::vector<vk::SampleCountFlagBits> &sampleCountFlagBits) {
                    return vklite::MaxMsaaSampleCountSelector(4).select(sampleCountFlagBits);
                });
        return *this;
    }
} // vklite
