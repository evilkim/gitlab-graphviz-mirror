if (with_ortho)

add_definitions(-D_BLD_gvc=1 -DGVC_EXPORTS)

add_library(ortho_obj OBJECT
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

target_include_directories(ortho_obj PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pack
    ${GRAPHVIZ_LIB_DIR}/pathplan
)

add_library(ortho STATIC
    $<TARGET_OBJECTS:ortho_obj>
)

endif (with_ortho)
