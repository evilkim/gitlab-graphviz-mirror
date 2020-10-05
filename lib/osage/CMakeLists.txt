add_library(osage STATIC
    # Header files
    osage.h

    # Source files
    osageinit.c
)

target_include_directories(osage PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pack
    ${GRAPHVIZ_LIB_DIR}/pathplan
)
