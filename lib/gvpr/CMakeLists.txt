add_library(gvpr STATIC
    # Header files
    actions.h
    compile.h
    gdefs.h
    gprdata.inc
    gprstate.h
    gvpr.h
    parse.h
    queue.h
    trieFA.h

    # Source files
    actions.c
    compile.c
    gvpr.c
    gprstate.c
    parse.c
    queue.c
)

target_include_directories(gvpr PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/.. # needed to find generated expr/exparse.h
    ${CMAKE_CURRENT_BINARY_DIR}/../common # needed to find generated common/colortbl.h
    ${GRAPHVIZ_LIB_DIR}/ast
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/expr
    ${Getopt_INCLUDE_DIRS}
)

target_link_libraries(gvpr
    ast
    cgraph
    ingraphs
    gvc
    expr
    sfio
    vmalloc
)

if(NOT HAVE_GETOPT_H)
    target_link_libraries(gvpr ${Getopt_LINK_LIBRARIES})
endif(NOT HAVE_GETOPT_H)
