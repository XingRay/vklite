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
        PRIVATE vklite sandbox math util
)

include("../../cmake/compile_copy_shader.cmake")
compile_copy_shader("shader" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/shader")
