
file(GLOB_RECURSE VKLITE_ENGINE_SRC_LIST
        *.cpp
        *.c++
        *.cxx
        *.c
)

add_library(vklite_engine
        ${VKLITE_ENGINE_SRC_LIST}
)

target_link_libraries(vklite_engine
        PUBLIC vklite_core
)

# 显式设置输出文件名（确保生成 vklite.lib 导入库）
set_target_properties(vklite_core PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)