BISON_TARGET(Grammar grammar.y ${CMAKE_CURRENT_BINARY_DIR}/grammar.c)
FLEX_TARGET(Scan scan.l  ${CMAKE_CURRENT_BINARY_DIR}/scan.c)
ADD_FLEX_BISON_DEPENDENCY(Scan Grammar)

add_definitions(-DEXPORT_CGRAPH -DEXPORT_AGXBUF -DEXPORT_CGHDR -DYY_NO_UNISTD_H)

add_library(cgraph SHARED
    # Header files
    agxbuf.h
    cghdr.h
    cgraph.h
    strcasecmp.h

    # Source files
    agerror.c
    agxbuf.c
    apply.c
    attr.c
    edge.c
    flatten.c
    graph.c
    id.c
    imap.c
    io.c
    mem.c
    node.c
    obj.c
    pend.c
    rec.c
    refstr.c
    subg.c
    utils.c
    write.c

    # Generated files
    ${BISON_Grammar_OUTPUTS}
    ${FLEX_Scan_OUTPUTS}
)

target_include_directories(cgraph PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(cgraph cdt)

# Installation location of library files
install(
    TARGETS cgraph
    RUNTIME DESTINATION ${BINARY_INSTALL_DIR}
    LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}
)

# Specify headers to be installed
install(
    FILES cgraph.h
    DESTINATION ${HEADER_INSTALL_DIR}
)

# Specify man pages to be installed
install(
    FILES cgraph.3
    DESTINATION ${MAN_INSTALL_DIR}
)

# Specify library version and soversion
set_target_properties(cgraph PROPERTIES
    VERSION 6.0.0
    SOVERSION 6
)
