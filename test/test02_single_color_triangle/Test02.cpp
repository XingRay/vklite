//
// Created by leixing on 2025-07-12.
//

#include "Test02.h"
#include "FileUtil.h"
#include "vklite_engine/graphic_pipeline_engine/WindowsGraphicPipelineEngineBuilder.h"

namespace test {
    Test02::Test02() {
        LOG_D("Test02::Test02()");
    }

    Test02::~Test02() = default;

    void Test02::init(GLFWwindow *window, int32_t width, int32_t height) {
        std::vector<uint32_t> vertexShaderCode = util::FileUtil::loadSpvFile("spv/02_triangle_color.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = util::FileUtil::loadSpvFile("spv/02_triangle_color.frag.spv");

        vklite::ShaderConfigure shaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(Vertex))
                            .addAttribute(0, ShaderFormat::Vec3);
                })
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addUniformBuffer([&](vklite::UniformBufferConfigure &uniformConfigure) {
                                uniformConfigure
                                        .binding(0)
                                        .descriptorCount(1)
                                        .shaderStageFlags(vk::ShaderStageFlagBits::eVertex);
                            });
                });

        mEngine = vklite::WindowsGraphicPipelineEngineBuilder::asDefault(window)
                .shaderConfigure(std::move(shaderConfigure))
                .clearColor(0.2f, 0.4f, 0.8f)
                .buildUnique();
        LOG_D("mEngine: build end");

        std::vector<Vertex> vertices = {
            {{1.0f,  -1.0f, 0.0f}},
            {{-1.0f, -1.0f, 0.0f}},
            {{0.0f,  1.0f,  0.0f}},
    };

        std::vector<uint32_t> indices = {0, 1, 2};

        ColorUniformBufferObject colorUniformBufferObject{{0.8f, 0.4f, 0.2f}};


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

        mUniformBuffers = mEngine->uniformBufferBuilder()
                .size(sizeof(ColorUniformBufferObject))
                .build(mEngine->getFrameCount());

        for (uint32_t i = 0; i < mEngine->getFrameCount(); i++) {
            mUniformBuffers[i].update(mEngine->getCommandPool(), &colorUniformBufferObject, sizeof(ColorUniformBufferObject));
        }

        mEngine->updateDescriptorSets([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &configure) {
            configure
                    .descriptorSet(mEngine->getDescriptorSets(frameIndex, 0))
                    .addUniformBuffer([&](vklite::UniformBufferDescriptorMapping &mapping) {
                        mapping
                                .addBufferInfo(mUniformBuffers[frameIndex].getBuffer());
                    });
        });
    }

    void Test02::drawFrame() {
        mEngine->drawIndexed();
    }

    void Test02::cleanup() {
    }

    void Test02::onWindowResized(int32_t width, int32_t height) {
    }
} // test01
