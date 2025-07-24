//
// Created by leixing on 2025-07-12.
//

#include "Test05.h"
#include "FileUtil.h"
#include "vklite_engine/graphic_pipeline_engine/WindowsGraphicPipelineEngineBuilder.h"

#include <numbers>
#include <cmath>

#include "ImageInterface.h"
#include "StbImage.h"
#include "vklite/plugin/validation/ValidationPlugin.h"

namespace test {
    Test05::Test05() {
        LOG_D("Test05::Test05()");
    }

    Test05::~Test05() = default;

    void Test05::init(GLFWwindow *window, int32_t width, int32_t height) {
        std::vector<uint32_t> vertexShaderCode = util::FileUtil::loadSpvFile("spv/05_texture_image.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = util::FileUtil::loadSpvFile("spv/05_texture_image.frag.spv");

        vklite::ShaderConfigure shaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(Vertex))
                            .addAttribute(0, ShaderFormat::Vec3)
                            .addAttribute(1, ShaderFormat::Vec2);
                })
                .addPushConstant(0, sizeof(glm::mat4), vk::ShaderStageFlagBits::eVertex)
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addCombinedImageSampler(0, vk::ShaderStageFlagBits::eFragment);
                });

        mEngine = vklite::WindowsGraphicPipelineEngineBuilder::asDefault(window)
                .shaderConfigure(std::move(shaderConfigure))
                .addInstancePlugin(vklite::ValidationPlugin::buildUnique())
                .clearColor(0.2f, 0.4f, 0.8f)
                .buildUnique();
        LOG_D("mEngine: build end");


        std::vector<Vertex> vertices = {
            {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}, // 左上
            {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}}, // 右上
            {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}, // 左下
            {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}, // 右下

        };

        std::vector<uint32_t> indices = {
            0, 2, 1, 1, 2, 3,
        };

        uint32_t indicesSize = indices.size() * sizeof(uint32_t);
        mIndexBuffer = mEngine->indexBufferBuilder()
                .size(indicesSize)
                .buildUnique();
        mIndexBuffer->update(mEngine->getCommandPool(), indices);
        mEngine->indexBuffer(*mIndexBuffer, indices.size());

        uint32_t verticesSize = vertices.size() * sizeof(Vertex);
        mVertexBuffer = mEngine->vertexBufferBuilder()
                .size(verticesSize)
                .buildUnique();
        mVertexBuffer->update(mEngine->getCommandPool(), vertices.data(), verticesSize);
        mEngine->addVertexBuffer(*mVertexBuffer);


        mMvpMatrix = MvpMatrix{};
        float scale = 1.0f;

        float screenWidth = static_cast<float>(width);
        float screenHeight = static_cast<float>(height);
        float aspectRatio = screenWidth / screenHeight;

        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        mMvpMatrix.model = model;
        mMvpMatrix.view = glm::lookAt(glm::vec3(0.0f, 5.0f, 5.0f),
                                      glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f));
        mMvpMatrix.proj = glm::perspective(glm::radians(45.0f),
                                           aspectRatio,
                                           1.0f,
                                           20.0f);
        mTimer.start();

        std::unique_ptr<image::ImageInterface> textureImage = image::StbImage::loadImage("resource/image/01.jpg", STBI_rgb_alpha);

        mSamplers = mEngine->samplerBuilder()
                .width(textureImage->getWidth())
                .height(textureImage->getHeight())
                .format(textureImage->getVkFormat())
                .build(mEngine->getFrameCount());

        for (uint32_t i = 0; i < mEngine->getFrameCount(); i++) {
            mSamplers[i].getImage().changeImageLayout(mEngine->getCommandPool());
            mSamplers[i].update(mEngine->getCommandPool(), textureImage->getPixels(), textureImage->getPixelBytes());
        }

        mEngine->updateDescriptorSets([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &configure) {
            configure
                    .descriptorSet(mEngine->getDescriptorSets(frameIndex, 0))
                    .addCombinedImageSampler([&](vklite::CombinedImageSamplerDescriptorMapping &mapping) {
                        mapping
                                .addImageInfo(mSamplers[frameIndex].getSampler(), mSamplers[frameIndex].getImageView());
                    });
        });
    }

    void Test05::drawFrame() {
        float time = mTimer.getElapsedTimeSecond();
        float scale = 1.0f;

        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
        model = glm::rotate(model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        mMvpMatrix.model = model;

        //        mMvpMatrix.model = glm::mat4(1.0f); // 单位矩阵
        //        mMvpMatrix.view = glm::mat4(1.0f);  // 单位矩阵
        //        mMvpMatrix.proj = glm::mat4(1.0f);  // 单位矩阵
        glm::mat4 mvp = mMvpMatrix.proj * mMvpMatrix.view * mMvpMatrix.model;

        mEngine->updatePushConstant(0, &mvp, sizeof(glm::mat4));

        mEngine->drawIndexed();
    }

    void Test05::cleanup() {
    }

    void Test05::onWindowResized(int32_t width, int32_t height) {
    }
} // test01
