add_library(ingraphs STATIC
    # Header files
    ingraphs.h

    # Source files
    ingraphs.c
)

target_include_directories(ingraphs PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
)
