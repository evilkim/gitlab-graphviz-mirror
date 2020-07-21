# This function sets some default values that every tools shares:
# - linking to getopt if REQUIRED
# - installation of executable
# - installation of manpage
function(tool_defaults name)
    if(NOT HAVE_GETOPT_H)
        target_link_libraries(${name} ${Getopt_LINK_LIBRARIES})
    endif(NOT HAVE_GETOPT_H)

    install(
        TARGETS ${name}
        RUNTIME DESTINATION ${BINARY_INSTALL_DIR}
    )

    install(
        FILES ${name}.1
        DESTINATION ${MAN_INSTALL_DIR}
    )
endfunction(tool_defaults)

# ================================ simple tools ================================
# These tools have a single source file and share linked libraries, etc.

# This function adds a simple tool using only it's name
function(add_simple_tool name)
    add_executable(${name} ${name}.c)

    target_include_directories(${name} PRIVATE
        ${GRAPHVIZ_LIB_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${GRAPHVIZ_LIB_DIR}/cdt
        ${GRAPHVIZ_LIB_DIR}/cgraph
        ${GRAPHVIZ_LIB_DIR}/common
        ${GRAPHVIZ_LIB_DIR}/gvc
        ${GRAPHVIZ_LIB_DIR}/pack
        ${GRAPHVIZ_LIB_DIR}/pathplan
        ${EXPAT_INCLUDE_DIRS}
        ${Getopt_INCLUDE_DIRS}
    )

    target_link_libraries(${name}
        cgraph
        ingraphs
    )

    tool_defaults(${name})
endfunction(add_simple_tool)

add_simple_tool(acyclic)
add_simple_tool(bcomps)
add_simple_tool(ccomps)
add_simple_tool(gc)
add_simple_tool(nop)
add_simple_tool(tred)
target_link_libraries(tred common) # e.g. for start_timer
add_simple_tool(unflatten)

# ================================ complex tools ===============================
# These tools have multiple source files, different dependencies or other
# differences from the simple tools above

# ================================== dijkstra ==================================
add_executable(dijkstra
    # Source files
    dijkstra.c
)

target_include_directories(dijkstra PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pack
    ${GRAPHVIZ_LIB_DIR}/pathplan
    ${EXPAT_INCLUDE_DIRS}
    ${Getopt_INCLUDE_DIRS}
)

target_link_libraries(dijkstra
    cdt
    cgraph
    ingraphs
)

tool_defaults(dijkstra)

# =================================== gml2gv ===================================
BISON_TARGET(Gmlparse gmlparse.y ${CMAKE_CURRENT_BINARY_DIR}/gmlparse.c)
FLEX_TARGET(Gmlscan gmlscan.l  ${CMAKE_CURRENT_BINARY_DIR}/gmlscan.c)
ADD_FLEX_BISON_DEPENDENCY(Gmlscan Gmlparse)

add_executable(gml2gv
    # Source files
    gml2gv.c

    # Generated files
    ${BISON_Gmlparse_OUTPUTS}
    ${FLEX_Gmlscan_OUTPUTS}
)

target_include_directories(gml2gv PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pack
    ${GRAPHVIZ_LIB_DIR}/pathplan
    ${EXPAT_INCLUDE_DIRS}
    ${Getopt_INCLUDE_DIRS}
)

target_link_libraries(gml2gv
    cgraph
    ingraphs
)

tool_defaults(gml2gv)

# Note: This is lifted almost wholesale from cmd/dot/CMakeLists.txt for setting up
# aliases to dot. This is overkill for a single alias.

# Aliases to the gml2gv executable (not including '.exe' suffix)
list(APPEND gml2gv_aliases gv2gml)
if(WIN32)
    # Copy gml2gv executable to each alias name then install copies to bindir
    foreach(cmd_alias IN LISTS gml2gv_aliases)
        set(gml2gvcopy "${CMAKE_CURRENT_BINARY_DIR}/${cmd_alias}${CMAKE_EXECUTABLE_SUFFIX}")
        add_custom_command(
            TARGET gml2gv
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:gml2gv> ${gml2gvcopy}
            COMMENT "Copying $<TARGET_FILE:gml2gv> to ${gml2gvcopy}"
        )
        install(
            PROGRAMS ${gml2gvcopy}
            DESTINATION ${BINARY_INSTALL_DIR}
        )
    endforeach()
else()
    # For not-WIN32, install symlinks from gml2gv_aliases -> gml2gv_executable in bindir.
    # Note: This may be brittle. This builds a symlink from ./cmd_alias -> ./gml2gv
    # in ${CMAKE_CURRENT_BINARY_DIR}, then installs that symlink
    # into ${BINARY_INSTALL_DIR}. This presumes ${CMAKE_CURRENT_BINARY_DIR}/gml2gv
    # is installed to ${BINARY_INSTALL_DIR}/gml2gv. There is a (small?) risk of
    # dangling symlinks
    foreach(cmd_alias IN LISTS gml2gv_aliases)
        set(gml2gvlink "${CMAKE_CURRENT_BINARY_DIR}/${cmd_alias}")
        add_custom_command(
            TARGET gml2gv
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_FILE_NAME:gml2gv> ${cmd_alias}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Linking $<TARGET_FILE_NAME:gml2gv> as ${cmd_alias}"
        )
        install(
            FILES ${gml2gvlink}
            DESTINATION ${BINARY_INSTALL_DIR}
        )
    endforeach()
endif()


# ================================= graphml2gv =================================
if(EXPAT_FOUND)

add_executable(graphml2gv
    # Source files
    graphml2gv.c
)

target_include_directories(graphml2gv PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pack
    ${GRAPHVIZ_LIB_DIR}/pathplan
    ${EXPAT_INCLUDE_DIRS}
    ${Getopt_INCLUDE_DIRS}
)

target_link_libraries(graphml2gv
    cdt
    cgraph
    ingraphs
    ${EXPAT_LIBRARIES}
)

tool_defaults(graphml2gv)

endif(EXPAT_FOUND)

# ================================== gvcolor ===================================
add_executable(gvcolor
    # Header files
    colortbl.h

    # Source files
    colxlate.c
    gvcolor.c
)

target_include_directories(gvcolor PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pack
    ${GRAPHVIZ_LIB_DIR}/pathplan
    ${EXPAT_INCLUDE_DIRS}
    ${Getopt_INCLUDE_DIRS}
)

target_link_libraries(gvcolor
    cgraph
    ingraphs
)

tool_defaults(gvcolor)

# ==================================== gvgen ===================================
add_executable(gvgen
    # Source files
    graph_generator.c
    gvgen.c
)

target_include_directories(gvgen PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pack
    ${GRAPHVIZ_LIB_DIR}/pathplan
    ${EXPAT_INCLUDE_DIRS}
    ${Getopt_INCLUDE_DIRS}
)

target_link_libraries(gvgen cgraph)

# Link to math library
if (UNIX)
    target_link_libraries(gvgen m)
endif (UNIX)

tool_defaults(gvgen)

# =================================== gvpack ===================================
# TODO add gvpack_static

add_definitions(-DDEMAND_LOADING=0)

add_executable(gvpack
    # Source files
    gvpack.c
)

target_include_directories(gvpack PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pack
    ${GRAPHVIZ_LIB_DIR}/pathplan
    ${EXPAT_INCLUDE_DIRS}
    ${Getopt_INCLUDE_DIRS}
)

target_link_libraries(gvpack
    cdt
    cgraph
    gvc
    gvplugin_neato_layout
    ingraphs
)

tool_defaults(gvpack)

# =================================== gxl2gv ===================================
if(EXPAT_FOUND)

add_executable(gxl2gv
    # Source files
    cvtgxl.c
    gv2gxl.c
    gxl2gv.c
)

target_include_directories(gxl2gv PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pack
    ${GRAPHVIZ_LIB_DIR}/pathplan
    ${EXPAT_INCLUDE_DIRS}
    ${Getopt_INCLUDE_DIRS}
)

target_link_libraries(gxl2gv
    cdt
    cgraph
    ingraphs
    ${EXPAT_LIBRARIES}
)

tool_defaults(gxl2gv)

endif(EXPAT_FOUND)

# ==================================== mm2gv ===================================
add_executable(mm2gv
    # Source files
    matrix_market.c
    mm2gv.c
    mmio.c
)

target_include_directories(mm2gv PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pack
    ${GRAPHVIZ_LIB_DIR}/pathplan
    ${EXPAT_INCLUDE_DIRS}
    ${Getopt_INCLUDE_DIRS}
)

target_link_libraries(mm2gv
    sparse
    cgraph
    common
    gvc
    pathplan
)

tool_defaults(mm2gv)

# =================================== sccmap ===================================
if(EXPAT_FOUND)

add_executable(sccmap
    # Source files
    sccmap.c
)

target_include_directories(sccmap PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pack
    ${GRAPHVIZ_LIB_DIR}/pathplan
    ${EXPAT_INCLUDE_DIRS}
    ${Getopt_INCLUDE_DIRS}
)

target_link_libraries(sccmap
    cgraph
    ingraphs
    ${EXPAT_LIBRARIES}
)

tool_defaults(sccmap)

endif(EXPAT_FOUND)

# ===================== Install third party DLLs on Windows ====================

if (WIN32 AND EXPAT_FOUND)
    install(
        FILES ${EXPAT_RUNTIME_LIBRARIES}
        DESTINATION ${BINARY_INSTALL_DIR}
    )
endif(WIN32 AND EXPAT_FOUND)

if (WIN32)
    install(
        FILES ${Getopt_RUNTIME_LIBRARIES}
        DESTINATION ${BINARY_INSTALL_DIR}
    )
endif()
