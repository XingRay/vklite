cmake_minimum_required(VERSION 3.31.6)
project(vklite
        LANGUAGES CXX
        VERSION 1.0.0
)

option(BUILD_TEST "build test" ON)

set(VULKAN_SDK_PATH "D:/develop/vulkan/VulkanSDK/1.4.313.2" CACHE STRING "vulkan sdk path")
set(glfw_PATH "D:/develop/opengl/glfw/glfw-3.4.bin.WIN64" CACHE STRING "glfw path")

# Vulkan-Headers
# https://github.com/KhronosGroup/Vulkan-Headers
set(VULKAN_HEADERS_PATH "D:/develop/vulkan/Vulkan-Headers/Vulkan-Headers-1.3.275" CACHE STRING
        "vulkan headers, used for android")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

if (WIN32)
    # Windows 特定编译选项
    add_compile_options(
             /W4 /WX

            # 关闭 C4100（未使用参数）警告
            /wd4100
    )

endif ()


add_subdirectory(src)

# 条件添加测试目录
if (BUILD_TEST)
    message("Enabling test build")
    add_subdirectory(test)
else ()
    message("Skipping test build")
endif ()