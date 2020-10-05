add_library(label STATIC
    # Header files
    index.h
    node.h
    rectangle.h
    split.q.h
    xlabels.h

    # Source files
    index.c
    node.c
    rectangle.c
    split.q.c
    xlabels.c
)

target_include_directories(label PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
)
