if (with_ortho)

add_definitions(-D_BLD_gvc=1)

add_library(ortho STATIC
    # Header files
    fPQ.h
    maze.h
    ortho.h
    partition.h
    rawgraph.h
    sgraph.h
    structures.h
    trap.h

    # Source files
    fPQ.c
    maze.c
    ortho.c
    partition.c
    rawgraph.c
    sgraph.c
    trapezoid.c
)

target_include_directories(ortho PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pack
    ${GRAPHVIZ_LIB_DIR}/pathplan
)

endif (with_ortho)
