cmake_minimum_required (VERSION 3.12 FATAL_ERROR)

find_package(Boost REQUIRED)
find_package(Catch2 REQUIRED)
find_package(RapidXml REQUIRED)
find_package(Svgpp REQUIRED)

enable_testing()

# for convenience, make a library that depends on everything so each
# separate test can be as small as possible
add_library(test_common STATIC
            catch2_main.cpp
            svgpp_context.cpp
            svgpp_context.h
            svgpp_document_traverser.cpp
            svgpp_document_traverser.h
            svg_analyzer.cpp
            svg_analyzer.h
            svg_analyzer_interface.h
            ../cmd/dot/dot_builtins.c
    )
set_target_properties(test_common PROPERTIES CXX_STANDARD 20)
set_target_properties(test_common PROPERTIES CXX_STANDARD_REQUIRED ON)
target_include_directories(test_common PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${GRAPHVIZ_LIB_DIR}
    ${RapidXml_INCLUDE_DIRS}
    ${Svgpp_INCLUDE_DIRS}
)
target_link_libraries(test_common PUBLIC Catch2::Catch2)
target_link_libraries(test_common PUBLIC
    gvplugin_core
    gvplugin_dot_layout
    gvplugin_gd
    gvplugin_neato_layout
    cgraph
    cgraph++
    gvc
    gvc++
)

macro(create_test testname)
    add_executable(test_${testname} test_${testname}.cpp)
    set_target_properties(test_${testname} PROPERTIES CXX_STANDARD 20)
    set_target_properties(test_${testname} PROPERTIES CXX_STANDARD_REQUIRED ON)
    add_test(NAME test_${testname} COMMAND test_${testname} --reporter junit --out test_${testname}.xml)
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
create_test(AGraph_construction)
create_test(GVContext_construction)
create_test(GVLayout_construction)
create_test(GVLayout_render)
create_test(GVContext_render_svg)
