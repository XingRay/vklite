 # glm
 # https://github.com/g-truc/glm
 set(GLM_INSTALL_DIR "D:/develop/opengl/glm/glm-1.0.1-light")

 add_library(glm INTERFACE)

 target_include_directories(glm
         INTERFACE ${GLM_INSTALL_DIR}
 )

 file(GLOB_RECURSE MATH_SRC_LIST
         *.cpp
 )

 add_library(math ${MATH_SRC_LIST})

 target_include_directories(math
         PUBLIC .
 )

 target_link_libraries(math
         PUBLIC glm
 )