if (with_sfdp)

add_library(sfdpgen STATIC
    # Header files
    Multilevel.h
    post_process.h
    PriorityQueue.h
    sfdp.h
    sfdpinternal.h
    sparse_solve.h
    spring_electrical.h
    stress_model.h
    uniform_stress.h

    # Source files
    Multilevel.c
    post_process.c
    PriorityQueue.c
    sfdpinit.c
    sparse_solve.c
    spring_electrical.c
    stress_model.c
    uniform_stress.c
)

target_include_directories(sfdpgen PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pack
    ${GRAPHVIZ_LIB_DIR}/pathplan
)

endif (with_sfdp)
