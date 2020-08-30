add_definitions(-DGVC_EXPORTS)

add_library(pack_obj OBJECT
    # Header files
    pack.h

    # Source files
    ccomps.c
    pack.c
)

target_include_directories(pack_obj PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pathplan
)

add_library(pack STATIC
    $<TARGET_OBJECTS:pack_obj>
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
