add_subdirectory(vklite)

if (${BUILD_ENGINE})
    # 添加 engine 工程
    add_subdirectory(vklite_engine)

    # 创建聚合库
    # calc 本身没有源码, 引入两个目标库的对象文件
    add_library(
            vklite STATIC
            $<TARGET_OBJECTS:vklite_core>
            $<TARGET_OBJECTS:vklite_engine>
    )

    target_link_libraries(vklite
            PUBLIC vklite_core vklite_engine
    )
else ()
    # 创建聚合库
    # calc 本身没有源码, 引入两个目标库的对象文件
    add_library(
            vklite STATIC
            $<TARGET_OBJECTS:vklite_core>
    )

    target_link_libraries(vklite
            PUBLIC vklite_core
    )
endif ()

# 显式设置输出文件名（确保生成 vklite.lib 导入库）
set_target_properties(vklite PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)


install(TARGETS vklite
        EXPORT VkliteModule

        # 静态库
        ARCHIVE DESTINATION "vklite/lib/$<CONFIG>"
        # 动态库
        LIBRARY DESTINATION "vklite/lib/$<CONFIG>"
        # 可执行文件 / windows dll文件
        RUNTIME DESTINATION "vklite/bin/$<CONFIG>"
)

# 导出目标是个虚拟的概念, 还需要真正的导出成文件才能被其他的用户使用
install(EXPORT VkliteModule
        # 导出的文件名 VkliteModule.cmake VkliteModule-release.cmake VkliteModule-debug.cmake
        FILE VkliteModule.cmake
        # 导出文件所在的目录(相对于 ${CMAKE_INSTALL_PREFIX}的目录)
        # 实际目录 ${CMAKE_INSTALL_PREFIX}/vklite/lib/cmake
        DESTINATION "vklite/lib/cmake"
)

configure_file(
        ${PROJECT_SOURCE_DIR}/cmake/VkliteConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/VkliteConfig.cmake

        # 此项可选，@ONLY 表示仅将 *.in 文件中的 @xxx@ 做替换，而 ${xxx} 不做替换
        @ONLY
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/VkliteConfig.cmake
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

if (${BUILD_ENGINE})
endif ()
