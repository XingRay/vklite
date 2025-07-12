# Android 平台 Vulkan 配置

message(STATUS "platform: android")
target_compile_definitions(${PROJECT_NAME} PRIVATE
        VKLITE_ANDROID
        ANDROID
        VK_USE_PLATFORM_ANDROID_KHR
)

# vulkan
# https://www.vulkan.org/
set(VULKAN_INSTALL_DIR D:/develop/vulkan/VulkanSDK/1.3.275.0)
set(VULKAN_INCLUDE_DIR ${VULKAN_INSTALL_DIR}/Include)

# Vulkan-Headers
# https://github.com/KhronosGroup/Vulkan-Headers
set(VULKAN_HEADERS_INSTALL_DIR D:/develop/vulkan/Vulkan-Headers/Vulkan-Headers-1.3.275)
set(VULKAN_HEADERS_INCLUDE_DIR ${VULKAN_HEADERS_INSTALL_DIR}/include)


function(configure_android_target target)
    target_include_directories(${PROJECT_NAME} PRIVATE
            # vulkan
             ${VULKAN_INCLUDE_DIR}

            # Vulkan-Hpp
            ${VULKAN_HEADERS_INCLUDE_DIR}
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
            android
            log
            vulkan
    )


endfunction()