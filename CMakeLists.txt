cmake_minimum_required(VERSION 3.21)
project(vklite LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 收集源代码
file(GLOB_RECURSE PROJECT_SOURCES
        "src/*.cpp"
        "src/*.c++"
        "src/*.c"
        "src/*.cc"
)

# 创建库
add_library(vklite SHARED
        ${PROJECT_SOURCES}
)

# 检测平台
if (ANDROID)
    include(cmake/vklite-android.cmake)
elseif (WIN32)
    include(cmake/vklite-windows.cmake)
else ()
    message(FATAL_ERROR "Unsupported platform!")
endif ()

# 设置头文件目录
target_include_directories(vklite PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

# 调用平台特定配置
if (ANDROID)
    configure_android_target(${PROJECT_NAME})
elseif (WIN32)
    configure_windows_target(${PROJECT_NAME})
endif ()

# 安装规则
install(TARGETS vklite
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

# 修改后的安装规则（适用于混合存放的情况）
install(DIRECTORY src/
        DESTINATION include
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"

        # 排除源文件
        PATTERN "*.cpp" EXCLUDE
        PATTERN "*.c++" EXCLUDE
        PATTERN "*.c" EXCLUDE
)

# 打包目标（可选）
add_custom_target(package
        COMMAND ${CMAKE_COMMAND} --install . --prefix ${CMAKE_BINARY_DIR}/dist
        COMMENT "Packaging library and headers..."
)