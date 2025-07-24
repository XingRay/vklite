//
// Created by leixing on 2025/5/13.
//

#pragma once

#include <cstdint>
#include <functional>

#include <vulkan/vulkan.hpp>

#include "vklite/render_pass/attachment/Attachment.h"
#include "vklite/render_pass/subpass/SubpassDescription.h"
#include "vklite/render_pass/subpass/SubpassDependency.h"

namespace vklite {
    class Subpass {
    private:
        uint32_t mIndex;

        // basic
        vk::SubpassDescriptionFlags mFlags;
        vk::PipelineBindPoint mPipelineBindPoint;

        // attachments
        std::vector<vk::AttachmentReference> mInputAttachments;
        std::vector<vk::AttachmentReference> mColorAttachments;
        std::vector<vk::AttachmentReference> mResolveAttachments;
        std::optional<vk::AttachmentReference> mDepthStencilAttachment;
        std::vector<vk::AttachmentReference> mPreserveAttachments;

        // dependencies
        std::vector<SubpassDependency> mDependencies;

    public:
        explicit Subpass();

        ~Subpass();

        Subpass(const Subpass &other) = delete;

        Subpass &operator=(const Subpass &other) = delete;

        Subpass(Subpass &&other) noexcept;

        Subpass &operator=(Subpass &&other) noexcept;

        Subpass &flags(vk::SubpassDescriptionFlags flags);

        Subpass &pipelineBindPoint(vk::PipelineBindPoint pipelineBindPoint);

        Subpass &index(uint32_t index);

        Subpass &addDependency(const Subpass &srcSubpass, vk::PipelineStageFlags srcStage, vk::AccessFlags srcAccess,
                               vk::PipelineStageFlags dstStage, vk::AccessFlags dstAccess, vk::DependencyFlags flags = vk::DependencyFlags{});

        Subpass &addInputAttachment(const Attachment &attachment, vk::ImageLayout layout);

        Subpass &addColorAttachment(const Attachment &attachment, vk::ImageLayout layout);

        Subpass &addResolveAttachment(const Attachment &attachment, vk::ImageLayout layout);

        Subpass &setDepthStencilAttachment(const Attachment &attachment, vk::ImageLayout layout);

        Subpass &addPreserveAttachment(const Attachment &attachment, vk::ImageLayout layout);

        [[nodiscard]]
        uint32_t getIndex() const;

        [[nodiscard]]
        SubpassDescription createSubpassDescription() const;

        [[nodiscard]]
        std::vector<vk::SubpassDependency> createSubpassDependencies() const;

    public: //static
        static Subpass externalSubpass();
    };
} // vklite
