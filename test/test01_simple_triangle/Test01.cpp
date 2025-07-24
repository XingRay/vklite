//
// Created by leixing on 2025-07-12.
//

#include "test01.h"
#include "FileUtil.h"
#include "vklite_engine/graphic_pipeline_engine/WindowsGraphicPipelineEngineBuilder.h"

namespace test {
    Test01::Test01() {
        LOG_D("Test01::Test01()");
    }

    Test01::~Test01() = default;

    void Test01::init(GLFWwindow *window, int32_t width, int32_t height) {
        LOG_D("Test01::init: window:%p, width:%d, height:%d", window, width, height);
        std::vector<uint32_t> vertexShaderCode = util::FileUtil::loadSpvFile("spv/01_triangle.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = util::FileUtil::loadSpvFile("spv/01_triangle.frag.spv");
        LOG_D("vertexShaderCode.size:%zd, fragmentShaderCode.size:%zd", vertexShaderCode.size(), fragmentShaderCode.size());

        vklite::ShaderConfigure shaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(Vertex))
                            .addAttribute(0, ShaderFormat::Vec3);
                });

        LOG_D("mEngine: build");
        mEngine = vklite::WindowsGraphicPipelineEngineBuilder::asDefault(window)
                .shaderConfigure(std::move(shaderConfigure))
                .clearColor(0.2f, 0.4f, 0.8f)
                .buildUnique();
        LOG_D("mEngine: build end");

        std::vector<Vertex> vertices = {
            {{1.0f, -1.0f, 0.0f}},
            {{-1.0f, -1.0f, 0.0f}},
            {{0.0f, 1.0f, 0.0f}},
        };

        std::vector<uint32_t> indices = {0, 1, 2};

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
    }

    void Test01::drawFrame() {
        mEngine->drawIndexed();
    }

    void Test01::cleanup() {
    }

    void Test01::onWindowResized(int32_t width, int32_t height) {
    }
} // test01
