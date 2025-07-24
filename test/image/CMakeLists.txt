 # stb
 set(STB_INSTALL_DIR "D:/develop/opengl/stb")

 add_library(stb INTERFACE)

 target_include_directories(stb
         INTERFACE ${STB_INSTALL_DIR}
 )

 file(GLOB_RECURSE IMAGE_SRC_LIST
         *.cpp
 )

 add_library(image ${IMAGE_SRC_LIST})

 target_include_directories(image
         PUBLIC .
 )

 target_link_libraries(image
         PUBLIC stb vklite
 )