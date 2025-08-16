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
        PRIVATE vklite sandbox math util image
)


include("../../cmake/compile_copy_shader.cmake")
compile_copy_shader(${CURRENT_TEST_NAME} "shader" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/shader")

include("../../cmake/utils.cmake")
copyDirToTargetDir(${CURRENT_TEST_NAME} "resource" "resource")

#set(TEST_IMAGES "resource/image/01.jpg")
#foreach (IMAGE_FILE ${TEST_IMAGES})
#    get_filename_component(IMAGE_FILE_NAME ${IMAGE_FILE} NAME)
#    add_custom_command(TARGET ${CURRENT_TEST_NAME} POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy
#            "${IMAGE_FILE}"
#            "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/resource/image/${IMAGE_FILE_NAME}"
#    )
#endforeach ()
