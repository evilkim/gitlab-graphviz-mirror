add_library(spine STATIC
    # Header files
    quad.h
    spine.h
    spinehdr.h
    subset.h
    union_find.h

    # Source files
    quad.c
    spine.c
    subset.c
    union_find.c
)

target_include_directories(spine PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
)
