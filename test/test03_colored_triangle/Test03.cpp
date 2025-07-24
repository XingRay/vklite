//
// Created by leixing on 2025-07-12.
//

#include "Test03.h"
#include "FileUtil.h"
#include "vklite_engine/graphic_pipeline_engine/WindowsGraphicPipelineEngineBuilder.h"

#include <numbers>
#include <cmath>

namespace test {
    Test03::Test03() {
        LOG_D("Test02::Test02()");
    }

    Test03::~Test03() = default;

    void Test03::init(GLFWwindow *window, int32_t width, int32_t height) {
        std::vector<uint32_t> vertexShaderCode = util::FileUtil::loadSpvFile("spv/03_colored_triangle.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = util::FileUtil::loadSpvFile("spv/03_colored_triangle.frag.spv");

        vklite::ShaderConfigure shaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(Vertex))
                            .addAttribute(0, ShaderFormat::Vec3)
                            .addAttribute(1, ShaderFormat::Vec3);
                });

        mEngine = vklite::WindowsGraphicPipelineEngineBuilder::asDefault(window)
                .shaderConfigure(std::move(shaderConfigure))
                .clearColor(0.2f, 0.4f, 0.8f)
                .buildUnique();
        LOG_D("mEngine: build end");


        // 六边形
        double r = 0.8;
        const int numVertices = 6;
        std::vector<Vertex> vertices;

        // 中心点
        vertices.push_back({
            {0.0f, 0.0f, 0.0f}, // 坐标
            {1.0f, 1.0f, 1.0f} // 颜色
        });
        std::vector<glm::vec3> colors = {
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 1.0f},
        };
        for (int i = 0; i < numVertices; ++i) {
            // 1. 计算角度（度 → 弧度）
            double angle_deg = i * 60.0; // 逆时针旋转60° × i
            double angle_rad = angle_deg * std::numbers::pi / 180.0; // 转为弧度

            // 2. 计算坐标
            double x = r * std::cos(angle_rad);
            double y = r * std::sin(angle_rad);

            // 4. 添加到顶点列表
            vertices.push_back({
                {static_cast<float>(x), static_cast<float>(y), 0.0f},
                colors[i]
            });
        }
        std::vector<uint32_t> indices = {
            0, 2, 1,
            0, 3, 2,
            0, 4, 3,
            0, 5, 4,
            0, 6, 5,
            0, 1, 6,
        };

        // triangle
        // std::vector<Vertex> vertices = {
        //     {{1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        //     {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        //     {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        // };
        //
        // std::vector<uint32_t> indices = {0, 1, 2};


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

    void Test03::drawFrame() {
        mEngine->drawIndexed();
    }

    void Test03::cleanup() {
    }

    void Test03::onWindowResized(int32_t width, int32_t height) {
    }
} // test01
