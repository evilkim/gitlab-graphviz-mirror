add_definitions(-DGVC_EXPORTS)

add_library(pack STATIC
    # Header files
    pack.h

    # Source files
    ccomps.c
    pack.c
)

target_include_directories(pack PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/neatogen
    ${GRAPHVIZ_LIB_DIR}/pathplan
)

# Specify headers to be installed
install(
    FILES pack.h
    DESTINATION ${HEADER_INSTALL_DIR}
)

# Specify man pages to be installed
install(
    FILES pack.3
    DESTINATION ${MAN_INSTALL_DIR}
)
