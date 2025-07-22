add_subdirectory(vklite)
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

# 显式设置输出文件名（确保生成 vklite.lib 导入库）
set_target_properties(vklite PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)
