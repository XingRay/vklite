
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

target_include_directories(vklite_core
        PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src> $<INSTALL_INTERFACE:vklite/include>
)

# 显式设置输出文件名（确保生成 vklite.lib 导入库）
set_target_properties(vklite_engine PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)

install(TARGETS vklite_engine
        EXPORT VkliteModule
)