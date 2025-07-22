//
// Created by leixing on 2025/1/4.
//

#pragma once

namespace test {
    class TestBase {
    public:
        explicit TestBase();

        virtual ~TestBase();

        virtual void init() = 0;

        virtual void drawFrame() = 0;

        virtual void cleanup() = 0;

        virtual void onWindowResized(int width, int height) =0;
    };
}
