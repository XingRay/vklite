//
// Created by leixing on 2025-07-12.
//

#pragma once

#include "TestBase.h"

namespace test {
    class Test01 : public TestBase {
    public:
        Test01();

        ~Test01() override;

        void init() override;

        void drawFrame() override;

        void cleanup() override;

        void onWindowResized(int width, int height) override;
    };
} // test01
