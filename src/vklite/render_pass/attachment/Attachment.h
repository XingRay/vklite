//
// Created by leixing on 2025/5/13.
//

#pragma once

#include <functional>

#include <vulkan/vulkan.hpp>

namespace vklite {
    class Subpass;

    class Attachment {
    private:
        uint32_t mIndex;

        vk::Format mFormat;
        vk::SampleCountFlagBits mSampleCount;
        vk::AttachmentLoadOp mLoadOp;
        vk::AttachmentStoreOp mStoreOp;
        vk::AttachmentLoadOp mStencilLoadOp;
        vk::AttachmentStoreOp mStencilStoreOp;
        // 整个 RenderPass 流程启动前附件的 Layout
        vk::ImageLayout mInitialLayout;
        // 整个 RenderPass 流程结束后附件的 Layout
        vk::ImageLayout mFinalLayout;

        // clear value
        vk::ClearValue mClearValue;

    public:
        explicit Attachment();

        ~Attachment();

        Attachment(const Attachment &other);

        Attachment &operator=(const Attachment &other);

        Attachment(Attachment &&other) noexcept;

        Attachment &operator=(Attachment &&other) noexcept;

        [[nodiscard]]
        uint32_t getIndex() const;

        [[nodiscard]]
        vk::ClearValue getClearValue() const;

        Attachment &format(vk::Format format);

        Attachment &sampleCount(vk::SampleCountFlagBits sampleCount);

        Attachment &loadOp(vk::AttachmentLoadOp loadOp);

        Attachment &storeOp(vk::AttachmentStoreOp storeOp);

        Attachment &stencilLoadOp(vk::AttachmentLoadOp stencilLoadOp);

        Attachment &stencilStoreOp(vk::AttachmentStoreOp stencilStoreOp);

        Attachment &initialLayout(vk::ImageLayout initialLayout);

        Attachment &finalLayout(vk::ImageLayout finalLayout);

        Attachment &index(uint32_t index);

        Attachment &asInputAttachmentUsedIn(Subpass &subpass, vk::ImageLayout layout = vk::ImageLayout::eShaderReadOnlyOptimal);

        Attachment &asInputAttachmentUsedInIf(bool condition, Subpass &subpass, vk::ImageLayout layout = vk::ImageLayout::eShaderReadOnlyOptimal);

        Attachment &asColorAttachmentUsedIn(Subpass &subpass, vk::ImageLayout layout = vk::ImageLayout::eColorAttachmentOptimal);

        Attachment &asColorAttachmentUsedInIf(bool condition, Subpass &subpass, vk::ImageLayout layout = vk::ImageLayout::eColorAttachmentOptimal);

        Attachment &asResolveAttachmentUsedIn(Subpass &subpass, vk::ImageLayout layout = vk::ImageLayout::eColorAttachmentOptimal);

        Attachment &asResolveAttachmentUsedInIf(bool condition, Subpass &subpass, vk::ImageLayout layout = vk::ImageLayout::eColorAttachmentOptimal);

        Attachment &asDepthStencilAttachmentUsedIn(Subpass &subpass, vk::ImageLayout layout = vk::ImageLayout::eDepthStencilAttachmentOptimal);

        Attachment &asDepthStencilAttachmentUsedInIf(bool condition, Subpass &subpass, vk::ImageLayout layout = vk::ImageLayout::eDepthStencilAttachmentOptimal);

        Attachment &asPreserveAttachmentUsedIn(Subpass &subpass, vk::ImageLayout layout);

        Attachment &asPreserveAttachmentUsedInIf(bool condition, Subpass &subpass, vk::ImageLayout layout);

        Attachment &applyIf(bool condition, const std::function<void(Attachment &attachment)> &handler);

        Attachment &clearColorValue(vk::ClearColorValue clearColorValue);

        Attachment &clearColorValue(const std::vector<float> &clearColorValue);

        Attachment &clearColorValue(const std::array<float, 4> &clearColorValue);

        Attachment &clearDepthStencilValue(vk::ClearDepthStencilValue clearDepthStencilValue);

        Attachment &clearDepthValue(float clearDepthValue);

        Attachment &clearStencilValue(uint32_t clearStencilValue);

        [[nodiscard]]
        vk::AttachmentDescription createAttachmentDescription() const;

    public: // static

        Attachment &msaaColorAttachment();

        Attachment &depthStencilAttachment();

        Attachment &presentColorAttachment();
    };
} // vklite
