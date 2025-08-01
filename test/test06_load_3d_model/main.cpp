//
// Created by leixing on 2025-07-22.
//

#include <memory>
#include "Sandbox.h"
#include "Test06.h"

int main(int argc, const char **argv) {
    std::unique_ptr<test::TestBase> test = std::make_unique<test::Test06>();
    test::Sandbox sandbox(std::move(test));
    sandbox.run();
}
