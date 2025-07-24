//
// Created by leixing on 2025/5/13.
//

#include "Subpass.h"

#include <format>

namespace vklite {
    Subpass::Subpass()
        : mIndex(0),
          mFlags(vk::SubpassDescriptionFlags{}),
          mPipelineBindPoint(vk::PipelineBindPoint::eGraphics),

          mInputAttachments{},
          mColorAttachments{},
          mResolveAttachments{},
          mDepthStencilAttachment{},
          mPreserveAttachments{},

          mDependencies{} {
    }

    Subpass::~Subpass() = default;

    Subpass::Subpass(Subpass &&other) noexcept
        : mIndex(std::exchange(other.mIndex, 0)),
          mFlags(std::exchange(other.mFlags, {})),
          mPipelineBindPoint(std::exchange(other.mPipelineBindPoint, {})),

          mInputAttachments(std::move(other.mInputAttachments)),
          mColorAttachments(std::move(other.mColorAttachments)),
          mResolveAttachments(std::move(other.mResolveAttachments)),
          mDepthStencilAttachment(std::move(other.mDepthStencilAttachment)),
          mPreserveAttachments(std::move(other.mPreserveAttachments)),

          mDependencies(std::move(other.mDependencies)) {
    }

    Subpass &Subpass::operator=(Subpass &&other) noexcept {
        if (this != &other) {
            mIndex = std::exchange(other.mIndex, 0);
            mFlags = std::exchange(other.mFlags, {});
            mPipelineBindPoint = std::exchange(other.mPipelineBindPoint, {});

            mInputAttachments = std::move(other.mInputAttachments);
            mColorAttachments = std::move(other.mColorAttachments);
            mResolveAttachments = std::move(other.mResolveAttachments);
            mDepthStencilAttachment = std::move(other.mDepthStencilAttachment);
            mPreserveAttachments = std::move(other.mPreserveAttachments);

            mDependencies = std::move(other.mDependencies);
        }
        return *this;
    }

    Subpass &Subpass::flags(vk::SubpassDescriptionFlags flags) {
        mFlags = flags;
        return *this;
    }

    Subpass &Subpass::pipelineBindPoint(vk::PipelineBindPoint pipelineBindPoint) {
        mPipelineBindPoint = pipelineBindPoint;
        return *this;
    }

    Subpass &Subpass::index(uint32_t index) {
        mIndex = index;
        return *this;
    }

    Subpass &Subpass::addDependency(const Subpass &srcSubpass, vk::PipelineStageFlags srcStage, vk::AccessFlags srcAccess,
                                    vk::PipelineStageFlags dstStage, vk::AccessFlags dstAccess, vk::DependencyFlags flags) {
        if (srcSubpass.mIndex != vk::SubpassExternal && srcSubpass.mIndex >= mIndex) {
            throw std::runtime_error(std::format("srcSubpass.mIndex({}) >= mIndex({})", srcSubpass.mIndex, mIndex));
        }

        mDependencies.emplace_back(SubpassDependencyItem(srcSubpass.mIndex, srcStage, srcAccess),
                                   SubpassDependencyItem(mIndex, dstStage, dstAccess),
                                   flags);
        return *this;
    }

    Subpass &Subpass::addInputAttachment(const Attachment &attachment, vk::ImageLayout layout) {
        mInputAttachments.emplace_back(attachment.getIndex(), layout);
        return *this;
    }

    Subpass &Subpass::addColorAttachment(const Attachment &attachment, vk::ImageLayout layout) {
        mColorAttachments.emplace_back(attachment.getIndex(), layout);
        return *this;
    }

    Subpass &Subpass::addResolveAttachment(const Attachment &attachment, vk::ImageLayout layout) {
        mResolveAttachments.emplace_back(attachment.getIndex(), layout);
        return *this;
    }

    Subpass &Subpass::setDepthStencilAttachment(const Attachment &attachment, vk::ImageLayout layout) {
        mDepthStencilAttachment = vk::AttachmentReference(attachment.getIndex(), layout);
        return *this;
    }

    Subpass &Subpass::addPreserveAttachment(const Attachment &attachment, vk::ImageLayout layout) {
        mPreserveAttachments.emplace_back(attachment.getIndex(), layout);
        return *this;
    }

    uint32_t Subpass::getIndex() const {
        return mIndex;
    }

    std::vector<uint32_t> convertPreserveAttachments(const std::vector<vk::AttachmentReference> &attachmentReferences) {
        std::vector<uint32_t> attachmentIndices;
        attachmentIndices.reserve(attachmentReferences.size());
        for (const vk::AttachmentReference &attachmentReference: attachmentReferences) {
            attachmentIndices.push_back(attachmentReference.attachment);
        }
        return attachmentIndices;
    }

    SubpassDescription Subpass::createSubpassDescription() const {
        return SubpassDescription{
            mFlags,
            mPipelineBindPoint,
            mInputAttachments,
            mColorAttachments,
            mResolveAttachments,
            mDepthStencilAttachment,
            convertPreserveAttachments(mPreserveAttachments)
        };
    }

    std::vector<vk::SubpassDependency> Subpass::createSubpassDependencies() const {
        std::vector<vk::SubpassDependency> subpassDependencies;
        subpassDependencies.reserve(mDependencies.size());
        for (const SubpassDependency &subpassDependency: mDependencies) {
            subpassDependencies.emplace_back(subpassDependency.createSubpassDependency());
        }

        return subpassDependencies;
    }

    Subpass Subpass::externalSubpass() {
        Subpass externalSubpass{};
        externalSubpass
                .index(vk::SubpassExternal);
        return externalSubpass;
    }
} // vklite
