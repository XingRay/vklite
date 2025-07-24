//
// Created by leixing on 2025-07-12.
//

#include "Test06.h"
#include "FileUtil.h"
#include "vklite_engine/graphic_pipeline_engine/WindowsGraphicPipelineEngineBuilder.h"

#include <numbers>
#include <cmath>
#include <thread>
#include <chrono>

#include "ImageInterface.h"
#include "Model.h"
#include "ModelLoader.h"
#include "StbImage.h"
#include "Vertex.h"
#include "vklite/plugin/validation/ValidationPlugin.h"

namespace test {
    Test06::Test06() {
        LOG_D("Test06::Test06()");
    }

    Test06::~Test06() = default;

    void Test06::init(GLFWwindow *window, int32_t width, int32_t height) {
        std::vector<uint32_t> vertexShaderCode = util::FileUtil::loadSpvFile("spv/06_load_3d_model.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = util::FileUtil::loadSpvFile("spv/06_load_3d_model.frag.spv");

        vklite::ShaderConfigure shaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(model::Vertex))
                            .addAttribute(0, ShaderFormat::Vec3)
                            .addAttribute(1, ShaderFormat::Vec2);
                })
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addUniformBuffer(0, vk::ShaderStageFlagBits::eVertex)
                            .addCombinedImageSampler(1, vk::ShaderStageFlagBits::eFragment);
                });

        mEngine = vklite::WindowsGraphicPipelineEngineBuilder::asDefault(window)
                .shaderConfigure(std::move(shaderConfigure))
                // .addInstancePlugin(vklite::ValidationPlugin::buildUnique())
                .clearColor(0.2f, 0.4f, 0.8f)
                .buildUnique();
        LOG_D("mEngine: build end");


        const char *MODEL_PATH = "resource/model/viking_room/viking_room.obj";
        const char *TEXTURE_PATH = "resource/model/viking_room/viking_room.png";

        model::Model model = model::ModelLoader::load(MODEL_PATH);
        std::unique_ptr<image::StbImage> textureImage = image::StbImage::loadImageAsRgba(TEXTURE_PATH);

        mIndexBuffer = mEngine->indexBufferBuilder()
                .size(model.getIndicesBytes())
                .buildUnique();
        mIndexBuffer->update(mEngine->getCommandPool(), model.getIndices());
        mEngine->indexBuffer(*mIndexBuffer, model.getIndicesCount());

        mVertexBuffer = mEngine->vertexBufferBuilder()
                .size(model.getVerticesBytes())
                .buildUnique();
        mVertexBuffer->update(mEngine->getCommandPool(), model.getVerticesData(), model.getVerticesBytes());
        mEngine->addVertexBuffer(*mVertexBuffer);


        mMvpMatrix = math::MvpMatrix{};
        float screenWidth = static_cast<float>(width);
        float screenHeight = static_cast<float>(height);
        float aspectRatio = screenWidth / screenHeight;

        mMvpMatrix.view(glm::lookAt(glm::vec3(0.0f, 5.0f, 5.0f),
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(0.0f, 0.0f, 1.0f)));
        mMvpMatrix.projection(glm::perspective(glm::radians(45.0f),
                                               aspectRatio,
                                               1.0f,
                                               20.0f));
        mMvpMatrix.projectionFlipY();
        mTimer.start();

        mSamplers = mEngine->samplerBuilder()
                .width(textureImage->getWidth())
                .height(textureImage->getHeight())
                .format(textureImage->getVkFormat())
                .build(mEngine->getFrameCount());

        for (uint32_t i = 0; i < mEngine->getFrameCount(); i++) {
            mSamplers[i].getImage().changeImageLayout(mEngine->getCommandPool());
            mSamplers[i].update(mEngine->getCommandPool(), textureImage->getPixels(), textureImage->getPixelBytes());
        }

        mUniformBuffers = mEngine->uniformBufferBuilder()
                .size(sizeof(math::MvpMatrix))
                .build(mEngine->getFrameCount());

        mEngine->updateDescriptorSets([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &configure) {
            configure
                    .descriptorSet(mEngine->getDescriptorSets(frameIndex, 0))
                    .addUniformBuffer([&](vklite::UniformBufferDescriptorMapping &mapping) {
                        mapping
                                .binding(0)
                                .addBufferInfo(mUniformBuffers[frameIndex].getBuffer());
                    })
                    .addCombinedImageSampler([&](vklite::CombinedImageSamplerDescriptorMapping &mapping) {
                        mapping
                                .binding(1)
                                .addImageInfo(mSamplers[frameIndex].getSampler(), mSamplers[frameIndex].getImageView());
                    });
        });
    }

    void Test06::drawFrame() {
        float deltaTime = mTimer.getDeltaTimeSecond();

        mMvpMatrix.modelRotateZ(deltaTime * glm::radians(90.0f));

        glm::mat4 mvp = mMvpMatrix.calcMvp();
        mUniformBuffers[mEngine->getFrameIndex()].update(mEngine->getCommandPool(), &mvp, sizeof(glm::mat4));

        mEngine->drawIndexed();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    void Test06::cleanup() {
    }

    void Test06::onWindowResized(int32_t width, int32_t height) {
    }
} // test01
