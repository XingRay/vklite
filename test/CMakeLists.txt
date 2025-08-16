# 添加测试用例的公共依赖
add_subdirectory(common)

set(CURRENT_TEST_NAME "test01_simple_triangle")
add_subdirectory(test01_simple_triangle)

set(CURRENT_TEST_NAME "test02_single_color_triangle")
add_subdirectory(test02_single_color_triangle)

set(CURRENT_TEST_NAME "test03_colored_triangle")
add_subdirectory(test03_colored_triangle)

set(CURRENT_TEST_NAME "test04_mvp_matrix")
add_subdirectory(test04_mvp_matrix)

## 自动添加测试
#file(GLOB dirs
#        *
#)
#message("test dirs:${dirs}")
#
## 添加所有测试用例
#foreach (dir_PATH IN LISTS dirs)
#    message("dir_PATH:${dir_PATH}")
#    get_filename_component(dir_NAME ${dir_PATH} NAME)
#    message("dir_NAME:${dir_NAME}")
#    if (IS_DIRECTORY ${dir_PATH} AND (${dir_NAME} MATCHES "^test"))
#        message("test dir_NAME:${dir_NAME}")
#        set(CURRENT_TEST_NAME ${dir_NAME})
#        add_subdirectory(${dir_PATH})
#    endif ()
#endforeach ()