file(GLOB_RECURSE SRC_LIST
        vklite/*.cpp
        vklite/*.cxx
        vklite/*.c++
        vklite/*.c
)

add_library(vklite ${SRC_LIST})

target_include_directories(vklite
        PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src> $<INSTALL_INTERFACE:vklite/include>
)


if (WIN32)
    message("build for windows")
    include(${PROJECT_SOURCE_DIR}/cmake/vklite-windows.cmake)

    target_link_libraries(vklite
            PUBLIC vulkan glfw
    )

    target_compile_definitions(vklite
            PUBLIC VKLITE_WINDOWS
    )

elseif (ANDROID)
    message("build for android")

    include(${PROJECT_SOURCE_DIR}/cmake/vklite-android.cmake)

    target_link_libraries(vklite
            PUBLIC
            android
            log
            vulkan
            vulkan_headers
    )

    target_compile_definitions(vklite
            PUBLIC VKLITE_ANDROID
    )

    #elseif (LINUX)
    # todo
    #elseif (MACOS)
    # todo
endif ()

# 显式设置输出文件名（确保生成 vklite.lib 导入库）
set_target_properties(vklite PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)


install(TARGETS vklite
        EXPORT vkliteModule

        # 静态库
        # vklite/lib/Debug  vklite/lib/Release
        ARCHIVE DESTINATION "vklite/lib/$<CONFIG>"

        # 动态库
        # vklite/lib/Debug  vklite/lib/Release
        LIBRARY DESTINATION "vklite/lib/$<CONFIG>"

        # 可执行文件 / windows dll文件
        # vklite/bin/Debug  vklite/bin/Release
        RUNTIME DESTINATION "vklite/bin/$<CONFIG>"
)

# 导出目标是个虚拟的概念, 还需要真正的导出成文件才能被其他的用户使用
install(EXPORT vkliteModule
        # 导出的文件名 VkliteModule.cmake VkliteModule-release.cmake VkliteModule-debug.cmake
        FILE vkliteModule.cmake
        # 导出文件所在的目录(相对于 ${CMAKE_INSTALL_PREFIX}的目录)
        # 实际目录 ${CMAKE_INSTALL_PREFIX}/vklite/lib/cmake
        DESTINATION "vklite/lib/cmake"
)

configure_file(
        ${PROJECT_SOURCE_DIR}/cmake/vkliteConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/vkliteConfig.cmake

        # 此项可选，@ONLY 表示仅将 *.in 文件中的 @xxx@ 做替换，而 ${xxx} 不做替换
        @ONLY
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/vkliteConfig.cmake
        DESTINATION "vklite/lib/cmake"

        # 打包相关
        COMPONENT "cmake"
)

install(DIRECTORY "${PROJECT_SOURCE_DIR}/src/"
        DESTINATION "vklite/include"
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
)