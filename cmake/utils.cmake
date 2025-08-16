
# 将指定目录(root_dir)下的所有子目录中符合 pattern 的子目录通过 add_subdirectory 指令添加到 cmake 中
function(add_subdirectories root_dir pattern)
    message("add_subdirectories: root_dir:${root_dir}, pattern:${pattern}")

    # 扫描根目录下的所有文件和目录
    file(GLOB dirs
            ${root_dir}/*
    )

    message("sub dirs:${dirs}")

    # 添加所有测试用例
    foreach (dir_PATH IN LISTS dirs)
        message("dir_PATH:${dir_PATH}")
        get_filename_component(dir_NAME ${dir_PATH} NAME)
        message("dir_NAME:${dir_NAME}")
        if (IS_DIRECTORY ${dir_PATH} AND (${dir_NAME} MATCHES ${pattern}))
            set(CURRENT_DIR_NAME ${dir_NAME})
            message("CURRENT_DIR_NAME: ${CURRENT_DIR_NAME}")
            add_subdirectory(${dir_PATH})
        endif ()
    endforeach ()

endfunction()

function(copyDirToTargetDir target_name src_dir dst_dir)
    message("copyDirToTargetDir: target_name:${target_name}, src_dir:${src_dir}, dst_dir:${dst_dir}")
    file(GLOB_RECURSE FILE_LIST ${src_dir}/*)
    foreach (FILE_NAME ${FILE_LIST})
        message("FILE_NAME:${FILE_NAME}")
    endforeach ()
    if (EXISTS ${src_dir} AND IS_DIRECTORY ${src_dir})

        #        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E remove_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${dirName}"
                COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${dirName}"
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_SOURCE_DIR}/${dirName}/"
                "$<TARGET_FILE_DIR:${target_name}>/${dirName}/"
                COMMENT "Copying ${dirName} directory to target directory"
        )

    endif ()

endfunction()

function(copyFileToTargetDir filePath)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${filePath}
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
            COMMENT "Copying file:[${filePath}] to target directory"
    )
endfunction()