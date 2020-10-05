if (ANN_FOUND)

add_library(libmingle STATIC
    # Header files
    agglomerative_bundling.h
    edge_bundling.h
    ink.h
    nearest_neighbor_graph.h
    nearest_neighbor_graph_ann.h

    # Source files
    agglomerative_bundling.c
    edge_bundling.c
    ink.c
    nearest_neighbor_graph.c
    nearest_neighbor_graph_ann.cpp
)

target_include_directories(libmingle PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pathplan
    ${ANN_INCLUDE_DIR}
)

endif (ANN_FOUND)
