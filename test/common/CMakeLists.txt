# 收集源代码
file(GLOB_RECURSE TEST_COMMON_SOURCES
        "*.cpp"
        "*.c++"
        "*.c"
        "*.cc"
)

# 创建公共库（包含common代码）
add_library(test_common STATIC
        ${TEST_COMMON_SOURCES}
)

target_include_directories(test_common
        PUBLIC .
)

target_link_libraries(test_common PUBLIC
        vklite glm
)