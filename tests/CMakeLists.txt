cmake_minimum_required (VERSION 3.12 FATAL_ERROR)

find_package(Catch2 REQUIRED)

enable_testing()

# for convenience, make a library that depends on everything so each
# separate test can be as small as possible
add_library(test_common STATIC
            catch2_main.cpp
            ../cmd/dot/dot_builtins.c
    )
set_target_properties(test_common PROPERTIES CXX_STANDARD 20)
set_target_properties(test_common PROPERTIES CXX_STANDARD_REQUIRED ON)
target_include_directories(test_common PRIVATE
    ${GRAPHVIZ_LIB_DIR}
)
target_link_libraries(test_common PUBLIC Catch2::Catch2)
target_link_libraries(test_common PUBLIC
    gvplugin_core
    gvplugin_dot_layout
    gvplugin_gd
    gvplugin_neato_layout
    cgraph
)

macro(create_test testname)
    add_executable(test_${testname} test_${testname}.cpp)
    set_target_properties(test_${testname} PROPERTIES CXX_STANDARD 20)
    set_target_properties(test_${testname} PROPERTIES CXX_STANDARD_REQUIRED ON)
    add_test(NAME test_${testname} COMMAND test_${testname})
    target_include_directories(test_${testname} PRIVATE
        ${GRAPHVIZ_LIB_DIR}
        ${GRAPHVIZ_LIB_DIR}/cdt
        ${GRAPHVIZ_LIB_DIR}/cgraph
        ${GRAPHVIZ_LIB_DIR}/common
        ${GRAPHVIZ_LIB_DIR}/gvc
        ${GRAPHVIZ_LIB_DIR}/pathplan
    )
    target_link_libraries(test_${testname} PRIVATE
        test_common
    )
endmacro()

create_test(simple)
