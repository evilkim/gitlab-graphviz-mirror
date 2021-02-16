add_executable(mkdefs
    # Source files
    mkdefs.c
)

# Generate gdefs.h from sources
if (WIN32)
    set (MKDEFS_EXECUTABLE "${CMAKE_CFG_INTDIR}/mkdefs")
else()
    set (MKDEFS_EXECUTABLE "./mkdefs")
endif()
add_custom_command(
    OUTPUT gdefs.h
    MAIN_DEPENDENCY gprdata
    DEPENDS mkdefs
    COMMAND ${MKDEFS_EXECUTABLE} gdefs.h < ${TOP_SOURCE_DIR}/lib/gvpr/gprdata
)

add_library(gvpr STATIC
    # Header files
    actions.h
    compile.h
    gdefs.h
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
