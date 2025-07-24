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
        PRIVATE vklite test_common math util
)

file(GLOB SPV_FILES
        "${PROJECT_SOURCE_DIR}/test/spv/01_*.spv"
)
foreach (SPV_FILE ${SPV_FILES})
    get_filename_component(SPV_FILE_NAME ${SPV_FILE} NAME)
    add_custom_command(TARGET ${CURRENT_TEST_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${SPV_FILE}"
            "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/spv/${SPV_FILE_NAME}"
    )
endforeach ()