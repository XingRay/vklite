//
// Created by leixing on 2025-07-12.
//

#include "test01.h"

#include <cstdio>

#include "../common/Sandbox.h"

namespace test {
    Test01::Test01() = default;

    Test01::~Test01() = default;

    void Test01::init() {
    }

    void Test01::drawFrame() {
    }

    void Test01::cleanup() {
    }

    void Test01::onWindowResized(int width, int height) {
    }
} // test01

int main(int argc, const char **argv) {
    std::unique_ptr<test::TestBase> test = std::make_unique<test::Test01>();
    test::Sandbox sandbox(std::move(test));
    sandbox.run();
}
