add_definitions(-DDEMAND_LOADING=1)

include_directories(
    ${GRAPHVIZ_LIB_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GRAPHVIZ_LIB_DIR}/cdt
    ${GRAPHVIZ_LIB_DIR}/cgraph
    ${GRAPHVIZ_LIB_DIR}/common
    ${GRAPHVIZ_LIB_DIR}/gvc
    ${GRAPHVIZ_LIB_DIR}/pathplan
)

add_executable(dot
    # Source files
    dot.c
    no_builtins.c
)

target_link_libraries(dot
    cgraph
    gvc
)

# Installation location of executables
install(
    TARGETS dot
    RUNTIME DESTINATION ${BINARY_INSTALL_DIR}
    LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}
)

# Aliases to the dot executable (not including '.exe' suffix)
list(APPEND dot_aliases circo fdp neato osage patchwork sfdp twopi)

# We use copying instead of symlinking for Cygwin to avoid
# https://gitlab.com/graphviz/graphviz/-/issues/2123

if(WIN32 OR CYGWIN)
    # Copy dot executable to each alias name then install copies to bindir
    foreach(cmd_alias IN LISTS dot_aliases)
        set(dotcopy "${CMAKE_CURRENT_BINARY_DIR}/${cmd_alias}${CMAKE_EXECUTABLE_SUFFIX}")
        add_custom_command(
            TARGET dot
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:dot> ${dotcopy}
            COMMENT "Copying $<TARGET_FILE:dot> to ${dotcopy}"
        )
        install(
            PROGRAMS ${dotcopy}
            DESTINATION ${BINARY_INSTALL_DIR}
        )
    endforeach()
else()
    # For not-WIN32, install symlinks from dot_aliases -> dot_executable in bindir.
    # Note: This may be brittle. This builds a symlink from ./cmd_alias -> ./dot
    # in ${CMAKE_CURRENT_BINARY_DIR}, then installs that symlink
    # into ${BINARY_INSTALL_DIR}. This presumes ${CMAKE_CURRENT_BINARY_DIR}/dot
    # is installed to ${BINARY_INSTALL_DIR}/dot. There is a (small?) risk of
    # dangling symlinks
    foreach(cmd_alias IN LISTS dot_aliases)
        set(dotlink "${CMAKE_CURRENT_BINARY_DIR}/${cmd_alias}")
        add_custom_command(
            TARGET dot
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_FILE_NAME:dot> ${cmd_alias}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Linking $<TARGET_FILE_NAME:dot> as ${cmd_alias}"
        )
        install(
            FILES ${dotlink}
            DESTINATION ${BINARY_INSTALL_DIR}
        )
    endforeach()
endif()

# Specify man pages to be installed
if(GZIP)
  add_custom_target(man-dot ALL DEPENDS dot.1.gz)
  add_custom_command(
    OUTPUT dot.1.gz
    COMMAND ${GZIP} -9 --no-name --to-stdout dot.1
      >"${CMAKE_CURRENT_BINARY_DIR}/dot.1.gz"
    MAIN_DEPENDENCY dot.1
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/dot.1.gz
    DESTINATION ${MAN_INSTALL_DIR}/man1)
  add_custom_target(man-osage ALL DEPENDS osage.1.gz)
  add_custom_command(
    OUTPUT osage.1.gz
    COMMAND ${GZIP} -9 --no-name --to-stdout osage.1
      >"${CMAKE_CURRENT_BINARY_DIR}/osage.1.gz"
    MAIN_DEPENDENCY osage.1
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/osage.1.gz
    DESTINATION ${MAN_INSTALL_DIR}/man1)
  add_custom_target(man-patchwork ALL DEPENDS patchwork.1.gz)
  add_custom_command(
    OUTPUT patchwork.1.gz
    COMMAND ${GZIP} -9 --no-name --to-stdout patchwork.1
      >"${CMAKE_CURRENT_BINARY_DIR}/patchwork.1.gz"
    MAIN_DEPENDENCY patchwork.1
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/patchwork.1.gz
    DESTINATION ${MAN_INSTALL_DIR}/man1)
else()
  install(
    FILES dot.1 osage.1 patchwork.1
    DESTINATION ${MAN_INSTALL_DIR}/man1
  )
endif()

install(
      SCRIPT ${TOP_SOURCE_DIR}/cmake/configure_plugins.cmake
)
