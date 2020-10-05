add_library(fdpgen STATIC
    # Header files
    clusteredges.h
    comp.h
    dbg.h
    fdp.h
    grid.h
    tlayout.h
    xlayout.h

    # Source files
    clusteredges.c
    comp.c
    dbg.c
    fdpinit.c
    grid.c
    layout.c
    tlayout.c
    xlayout.c
)

target_include_directories(fdpgen PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pack
    ${GRAPHVIZ_LIB_DIR}/pathplan
)
