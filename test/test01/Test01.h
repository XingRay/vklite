//
// Created by leixing on 2025-07-12.
//

#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "TestBase.h"
#include "glm.h"

#include "vklite/vklite.h"
#include "vklite/Log.h"

#include "vklite_engine/engines.h"


namespace test {
    struct Vertex {
        glm::vec3 pos;
    };

    class Test01 : public TestBase {
    private:
        std::unique_ptr<vklite::GraphicPipelineEngine> mEngine;
        std::unique_ptr<vklite::IndexBuffer> mIndexBuffer;
        std::unique_ptr<vklite::VertexBuffer> mVertexBuffer;

    public:
        Test01();

        ~Test01() override;

        void init(GLFWwindow *window, int32_t width, int32_t height) override;

        void drawFrame() override;

        void cleanup() override;

        void onWindowResized(int width, int height) override;
    };
} // test01
