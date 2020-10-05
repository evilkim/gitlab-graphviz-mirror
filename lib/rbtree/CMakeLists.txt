add_library(rbtree STATIC
    # Header files
    red_black_tree.h
    stack.h

    # Source files
    misc.c
    red_black_tree.c
    stack.c
)

include_directories(rbtree PRIVATE
    ${GRAPHVIZ_LIB_DIR}
)
