add_library(patchwork STATIC
    # Header files
    patchwork.h
    tree_map.h

    # Source files
    patchwork.c
    patchworkinit.c
    tree_map.c
)

target_include_directories(patchwork PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pack
    ${GRAPHVIZ_LIB_DIR}/pathplan
)
