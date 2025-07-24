//
// Created by leixing on 2025-07-12.
//

#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "TestBase.h"
#include "glm.h"
#include "MvpMatrix.h"
#include "Timer.h"

#include "vklite/vklite.h"
#include "vklite/Log.h"

#include "vklite_engine/engines.h"


namespace test {

    struct UniformBufferObject {
        float deltaTime = 1.0f;
    };

    struct Particle {
        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec4 color;
    };

    class Test07 : public TestBase {
    private:
        std::unique_ptr<vklite::ComputeGraphicEngine> mEngine;
        std::unique_ptr<vklite::VertexBuffer> mVertexBuffer;
        std::unique_ptr<vklite::IndexBuffer> mIndexBuffer;

        static constexpr uint32_t mParticleCount = 8192;
        //status
        util::Timer mTimer;
        std::vector<vklite::StorageBuffer> mShaderStorageBuffers;
        std::vector<vklite::UniformBuffer> mUniformBuffers;

    public:
        Test07();

        ~Test07() override;

        void init(GLFWwindow *window, int32_t width, int32_t height) override;

        void drawFrame() override;

        void cleanup() override;

        void onWindowResized(int width, int height) override;
    };
} // test01
