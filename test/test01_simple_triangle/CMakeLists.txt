# build test
message("build: ${CURRENT_TEST_NAME}")
message("PROJECT_NAME: ${PROJECT_NAME}")
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

# 收集源代码
file(GLOB_RECURSE TEST_SOURCES
        "*.cpp"
        "*.c++"
        "*.c"
        "*.cc"
)

add_executable(${CURRENT_TEST_NAME}
        ${TEST_SOURCES}
)

target_link_libraries(${CURRENT_TEST_NAME}
        PRIVATE vklite sandbox math util
)

# 1. 定义着色器类型扩展名列表
set(SHADER_TYPES "*.frag" "*.vert" "*.comp" "*.geom" "*.tesc" "*.tese" "*.rgen" "*.rchit" "*.rmiss" "*.mesh" "*.task")

# 2. 递归收集所有着色器文件
file(GLOB_RECURSE SHADER_FILES
        ${SHADER_TYPES}
)

# 3. 设置输出基础目录（支持不同构建配置）
set(SHADER_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/shader")

# 4. 遍历处理每个着色器文件
foreach(SHADER_FILE ${SHADER_FILES})
    # 获取文件名（不含路径）
    get_filename_component(SHADER_FILE_NAME ${SHADER_FILE} NAME)

    # 生成SPV文件名（保留原扩展名）
    set(SPV_FILE_NAME "${SHADER_FILE_NAME}.spv")

    # 拼接完整输出路径
    set(OUTPUT_PATH "${SHADER_OUTPUT_DIR}/${SPV_FILE_NAME}")

    # 添加自定义编译命令
    add_custom_command(
            TARGET ${CURRENT_TEST_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${SHADER_OUTPUT_DIR}"  # 自动创建目录
            COMMAND glslc "${SHADER_FILE}" -o "${OUTPUT_PATH}"
            COMMENT "Compiling shader: ${SHADER_FILE_NAME}"
            VERBATIM
    )
endforeach()