//
// Created by leixing on 2025-07-12.
//

#include "Test07.h"
#include "FileUtil.h"
#include "vklite_engine/graphic_pipeline_engine/WindowsGraphicPipelineEngineBuilder.h"

#include <numbers>
#include <cmath>
#include <thread>
#include <chrono>
#include <random>

#include "vklite/plugin/validation/ValidationPlugin.h"
#include "vklite_engine/compute_graphic_engine/WindowsComputeGraphicEngineBuilder.h"

namespace test {
    Test07::Test07() {
        LOG_D("Test07::Test07()");
    }

    Test07::~Test07() = default;

    void Test07::init(GLFWwindow *window, int32_t width, int32_t height) {
        std::vector<uint32_t> computeShaderCode = util::FileUtil::loadSpvFile("spv/07_compute.comp.spv");
        std::vector<uint32_t> vertexShaderCode = util::FileUtil::loadSpvFile("spv/07_compute.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = util::FileUtil::loadSpvFile("spv/07_compute.frag.spv");

        // Initialize particles
        std::default_random_engine rndEngine((unsigned) time(nullptr));
        std::uniform_real_distribution<float> rndDist(0.0f, 1.0f);

        // Initial particle positions on a circle
        std::vector<Particle> particles(mParticleCount);
        for (auto &particle: particles) {
            float r = 0.25f * sqrt(rndDist(rndEngine));
            float theta = rndDist(rndEngine) * 2.0f * 3.14159265358979323846f;
            float x = r * cos(theta) * height / width;
            float y = r * sin(theta);
            particle.position = glm::vec2(x, y);
            particle.velocity = glm::normalize(glm::vec2(x, y)) * 0.00025f;
            particle.color = glm::vec4(rndDist(rndEngine), rndDist(rndEngine), rndDist(rndEngine), 1.0f);
        }

        VkDeviceSize shaderStorageBufferSize = sizeof(Particle) * mParticleCount;


        vklite::ShaderConfigure graphicShaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(Particle))
                            //                            .inputRate(vk::VertexInputRate::eVertex)
                            .addAttribute(0, ShaderFormat::Vec2, offsetof(Particle, position))
                            .addAttribute(1, ShaderFormat::Vec4, offsetof(Particle, color));
                });

        vklite::ShaderConfigure computeShaderConfigure = vklite::ShaderConfigure()
                .computeShaderCode(std::move(computeShaderCode))
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addUniformBuffer(0, 1, vk::ShaderStageFlagBits::eCompute)
                            .addStorageBuffer(1, 1, vk::ShaderStageFlagBits::eCompute)
                            .addStorageBuffer(2, 1, vk::ShaderStageFlagBits::eCompute);
                });

        mEngine = vklite::WindowsComputeGraphicEngineBuilder::asDefault(window)
                .clearColor(0.2f, 0.4f, 0.8f)
                .computeShaderConfigure(std::move(computeShaderConfigure))
                .graphicShaderConfigure(std::move(graphicShaderConfigure))
                .buildUnique();

        LOG_D("mEngine: build end");


        for (uint32_t i = 0; i < mEngine->getFrameCount(); i++) {
            vklite::StorageBuffer storageBuffer = mEngine->storageBufferBuilder()
                    .size(shaderStorageBufferSize)
                    .addUsage(vk::BufferUsageFlagBits::eVertexBuffer)
                    .build();
            storageBuffer.update(mEngine->getCommandPool(), particles.data(), shaderStorageBufferSize);
            mShaderStorageBuffers.push_back(std::move(storageBuffer));

            vklite::UniformBuffer uniformBuffer = mEngine->uniformBufferBuilder()
                    .size(sizeof(UniformBufferObject))
                    .build();

            mUniformBuffers.push_back(std::move(uniformBuffer));
        }

        mEngine->addVertexBuffers([&](uint32_t frameIndex, std::vector<vk::Buffer> &buffers, std::vector<vk::DeviceSize> &offsets) {
            buffers.push_back(mShaderStorageBuffers[frameIndex].getVkBuffer());
            offsets.push_back(0);
        });


        mEngine->updateDescriptorSets([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &configure) {
            configure
                    .descriptorSet(mEngine->getComputeDescriptorSets(frameIndex, 0))
                    .addMapping([&](vklite::DescriptorMapping &mapping) {
                        mapping
                                .binding(0)
                                .descriptorType(vk::DescriptorType::eUniformBuffer)
                                .addBufferInfo(mUniformBuffers[frameIndex].getCombinedMemoryBuffer().getBuffer());
                    })
                    .addMapping([&](vklite::DescriptorMapping &mapping) {
                        mapping
                                .binding(1)
                                .descriptorType(vk::DescriptorType::eStorageBuffer)
                                .addBufferInfo(mShaderStorageBuffers[(frameIndex - 1) % mEngine->getFrameCount()].getVkBuffer(), 0, (vk::DeviceSize) shaderStorageBufferSize);
                    })
                    .addMapping([&](vklite::DescriptorMapping &mapping) {
                        mapping
                                .binding(2)
                                .descriptorType(vk::DescriptorType::eStorageBuffer)
                                .addBufferInfo(mShaderStorageBuffers[frameIndex].getVkBuffer(), 0, (vk::DeviceSize) shaderStorageBufferSize);
                    });
        });
        mTimer.start();
    }

    void Test07::drawFrame() {
        UniformBufferObject ubo{};
        ubo.deltaTime = mTimer.getDeltaTimeMs() * 2.0f;
        //        LOG_D("ubo.deltaTime: %f", ubo.deltaTime);
        mUniformBuffers[mEngine->getFrameIndex()].update(mEngine->getCommandPool(), &ubo, sizeof(UniformBufferObject));

        mEngine->drawIndexed(mParticleCount);

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    void Test07::cleanup() {
    }

    void Test07::onWindowResized(int32_t width, int32_t height) {
    }
} // test01
