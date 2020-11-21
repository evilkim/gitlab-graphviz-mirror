add_definitions(-DEXPORT_GVPR)

add_executable(gvpr-bin
    # Source files
    gvprmain.c
)

target_include_directories(gvpr-bin PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/gvpr
)

target_link_libraries(gvpr-bin
    ast
    cgraph
    gvc
    gvpr
    expr
    sfio
)

set_target_properties(gvpr-bin
  PROPERTIES OUTPUT_NAME gvpr)

# Installation location of executables
install(
    TARGETS gvpr-bin
    RUNTIME DESTINATION ${BINARY_INSTALL_DIR}
    LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}
)

# Specify man pages to be installed
install(
    FILES gvpr.1
    DESTINATION ${MAN_INSTALL_DIR}
)
