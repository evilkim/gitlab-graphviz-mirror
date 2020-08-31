if (with_ipsepcola)

add_library(vpsc STATIC
    # Header files
    block.h
    blocks.h
    constraint.h
    csolve_VPSC.h
    generate-constraints.h
    pairingheap/PairingHeap.h
    pairingheap/dsexceptions.h
    remove_rectangle_overlap.h
    solve_VPSC.h
    variable.h

    # Source files
    block.cpp
    blocks.cpp
    constraint.cpp
    csolve_VPSC.cpp
    generate-constraints.cpp
    pairingheap/PairingHeap.cpp
    remove_rectangle_overlap.cpp
    solve_VPSC.cpp
    variable.cpp
)

target_include_directories(vpsc PRIVATE
    ${GRAPHVIZ_LIB_DIR}
)

endif (with_ipsepcola)
