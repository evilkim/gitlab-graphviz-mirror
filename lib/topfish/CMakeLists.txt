if (with_smyrna)

add_library(topfish STATIC
    # Header files
    hierarchy.h

    # Source files
    hierarchy.c
    rescale_layout.c
)

target_include_directories(topfish PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/neatogen
)

endif (with_smyrna)
