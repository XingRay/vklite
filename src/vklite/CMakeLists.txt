message("PROJECT_SOURCE_DIR:${PROJECT_SOURCE_DIR}")


file(GLOB_RECURSE VKLITE_CORE_SRC_LIST
        *.cpp
        *.cxx
        *.c++
        *.c
)

add_library(vklite_core ${VKLITE_CORE_SRC_LIST})


if (WIN32)
    message("build for windows")
    include(${PROJECT_SOURCE_DIR}/cmake/vklite-windows.cmake)
    import_vulkan()
    import_glfw()

    target_link_libraries(vklite_core
            PUBLIC Vulkan glfw
    )

elseif (ANDROID)
    message("build for android")

    include(${PROJECT_SOURCE_DIR}/cmake/vklite-android.cmake)
    import_vulkan()

    target_link_libraries(vklite_core
            PUBLIC Vulkan VulkanHeaders
    )

    #elseif (LINUX)
    # todo
    #elseif (MACOS)
    # todo
endif ()


target_include_directories(vklite_core
        PUBLIC ../
)

if (WIN32)
    target_link_libraries(vklite_core
            PUBLIC glfw
    )

    target_compile_options(vklite_core
            PUBLIC /W4 /WX
    )

    target_compile_definitions(vklite_core
            PUBLIC VKLITE_WINDOWS
    )
elseif (ANDROID)
    target_link_libraries(vklite_core
            PUBLIC
    )

    target_compile_definitions(vklite_core
            PUBLIC VKLITE_ANDROID
    )
endif ()

# 显式设置输出文件名（确保生成 vklite.lib 导入库）
set_target_properties(vklite_core PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)
