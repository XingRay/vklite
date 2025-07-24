# tinyobjloader
set(TINYOBJLOADER_INSTALL_DIR D:/develop/opengl/tinyobjloader)
set(TINYOBJLOADER_INCLUE_DIR ${TINYOBJLOADER_INSTALL_DIR})

add_library(tiny_obj_loader INTERFACE)

target_include_directories(tiny_obj_loader
        INTERFACE ${TINYOBJLOADER_INCLUE_DIR}
)

file(GLOB_RECURSE MODEL_SRC_LIST
        *.cpp
)

add_library(model ${MODEL_SRC_LIST})

target_include_directories(model
        PUBLIC .
)

target_link_libraries(model
        PUBLIC tiny_obj_loader math
)