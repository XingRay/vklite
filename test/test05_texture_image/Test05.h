//
// Created by leixing on 2025-07-12.
//

#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "TestBase.h"
#include "glm.h"
#include "Timer.h"

#include "vklite/vklite.h"
#include "vklite/Log.h"

#include "vklite_engine/engines.h"


namespace test {
    struct Vertex {
        glm::vec3 position;
        glm::vec2 uv;
    };

    struct MvpMatrix {
        glm::mat4 model; // 模型矩阵
        glm::mat4 view; // 视图矩阵
        glm::mat4 proj; // 投影矩阵
    };

    class Test05 : public TestBase {
    private:
        std::unique_ptr<vklite::GraphicPipelineEngine> mEngine;
        std::unique_ptr<vklite::VertexBuffer> mVertexBuffer;
        std::unique_ptr<vklite::IndexBuffer> mIndexBuffer;

        MvpMatrix mMvpMatrix{};
        util::Timer mTimer;

        std::vector<vklite::CombinedImageSampler> mSamplers;

    public:
        Test05();

        ~Test05() override;

        void init(GLFWwindow *window, int32_t width, int32_t height) override;

        void drawFrame() override;

        void cleanup() override;

        void onWindowResized(int width, int height) override;
    };
} // test01
