//
// Created by leixing on 2025/5/14.
//

#include "SubpassDescription.h"

namespace vklite {
    SubpassDescription::SubpassDescription(
        vk::SubpassDescriptionFlags flags,
        vk::PipelineBindPoint pipelineBindPoint,
        std::vector<vk::AttachmentReference> inputAttachments,
        std::vector<vk::AttachmentReference> colorAttachments,
        std::vector<vk::AttachmentReference> resolveAttachments,
        std::optional<vk::AttachmentReference> depthStencilAttachment,
        std::vector<uint32_t> preserveAttachments)
        : mFlags(flags),
          mPipelineBindPoint(pipelineBindPoint),
          mInputAttachments(std::move(inputAttachments)),
          mColorAttachments(std::move(colorAttachments)),
          mResolveAttachments(std::move(resolveAttachments)),
          mDepthStencilAttachment(depthStencilAttachment),
          mPreserveAttachments(std::move(preserveAttachments)) {
    }

    SubpassDescription::~SubpassDescription() = default;

    SubpassDescription::SubpassDescription(const SubpassDescription &other)
        : mFlags(other.mFlags),
          mPipelineBindPoint(other.mPipelineBindPoint),
          mInputAttachments(other.mInputAttachments),
          mColorAttachments(other.mColorAttachments),
          mResolveAttachments(other.mResolveAttachments),
          mDepthStencilAttachment(other.mDepthStencilAttachment),
          mPreserveAttachments(other.mPreserveAttachments) {
    }

    SubpassDescription &SubpassDescription::operator=(const SubpassDescription &other) {
        if (this != &other) {
            mFlags = other.mFlags;
            mPipelineBindPoint = other.mPipelineBindPoint;
            mInputAttachments = other.mInputAttachments;
            mColorAttachments = other.mColorAttachments;
            mResolveAttachments = other.mResolveAttachments;
            mDepthStencilAttachment = other.mDepthStencilAttachment;
            mPreserveAttachments = other.mPreserveAttachments;
        }
        return *this;
    }

    SubpassDescription::SubpassDescription(SubpassDescription &&other) noexcept
        : mFlags(std::exchange(other.mFlags, {})),
          mPipelineBindPoint(std::exchange(other.mPipelineBindPoint, {})),
          mInputAttachments(std::move(other.mInputAttachments)),
          mColorAttachments(std::move(other.mColorAttachments)),
          mResolveAttachments(std::move(other.mResolveAttachments)),
          mDepthStencilAttachment(std::move(other.mDepthStencilAttachment)),
          mPreserveAttachments(std::move(other.mPreserveAttachments)) {
    }

    SubpassDescription &SubpassDescription::operator=(SubpassDescription &&other) noexcept {
        if (this != &other) {
            mFlags = std::exchange(other.mFlags, {});
            mPipelineBindPoint = std::exchange(other.mPipelineBindPoint, {});
            mInputAttachments = std::move(other.mInputAttachments);
            mColorAttachments = std::move(other.mColorAttachments);
            mResolveAttachments = std::move(other.mResolveAttachments);
            mDepthStencilAttachment = std::move(other.mDepthStencilAttachment);
            mPreserveAttachments = std::move(other.mPreserveAttachments);
        }
        return *this;
    }

    vk::SubpassDescription SubpassDescription::createSubpassDescription() const {
        vk::SubpassDescription subpassDescription{};
        subpassDescription
                .setFlags(mFlags)
                .setPipelineBindPoint(mPipelineBindPoint)
                .setInputAttachments(mInputAttachments)
                .setPreserveAttachments(mPreserveAttachments);

        if (!mColorAttachments.empty()) {
            subpassDescription.setColorAttachments(mColorAttachments);
        }
        if (!mResolveAttachments.empty()) {
            // ** setResolveAttachments 会同时设置 colorAttachmentCount, 而不存在 resolveAttachmentCount
            subpassDescription.setResolveAttachments(mResolveAttachments);
        }

        if (mDepthStencilAttachment.has_value()) {
            subpassDescription.setPDepthStencilAttachment(&(mDepthStencilAttachment.value()));
        }

        return subpassDescription;
    }
} // vklite
