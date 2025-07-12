# Windows 平台 Vulkan SDK 配置

message(STATUS "platform: windows")
target_compile_definitions(${PROJECT_NAME} PRIVATE VKLITE_WINDOWS)

if(NOT DEFINED ENV{VULKAN_SDK})
    message(FATAL_ERROR "VULKAN_SDK environment variable not set!")
endif()

set(VULKAN_INCLUDE_DIR "$ENV{VULKAN_SDK}/Include")
set(VULKAN_LIBRARY "$ENV{VULKAN_SDK}/Lib/vulkan-1.lib")

# glfw
set(GLFW_INSTALL_DIR D:/develop/opengl/glfw/glfw-3.4.bin.WIN64)
set(GLFW_LIB_DIR ${GLFW_INSTALL_DIR}/lib-vc2022)
set(GLFW_INCLUDE_DIR ${GLFW_INSTALL_DIR}/include)


function(configure_windows_target target)
    target_include_directories(${target} PRIVATE
            # Vulkan
            ${VULKAN_INCLUDE_DIR}

            # GLFW
            ${GLFW_INCLUDE_DIR}
    )

    target_link_libraries(${target} PRIVATE
            # Vulkan
            ${VULKAN_LIBRARY}

            # GLFW
            ${GLFW_LIB_DIR}/glfw3.lib
    )

    # Windows 特定编译选项
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /WX)
    endif()
endfunction()