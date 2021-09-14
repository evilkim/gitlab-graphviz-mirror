add_definitions(-DEXPORT_XDOT)

add_library(xdot SHARED
    # Header files
    xdot.h

    # Source files
    xdot.c
)

target_include_directories(xdot PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Installation location of library files
install(
    TARGETS xdot
    RUNTIME DESTINATION ${BINARY_INSTALL_DIR}
    LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}
)

# Specify headers to be installed
install(
    FILES xdot.h
    DESTINATION ${HEADER_INSTALL_DIR}
)

# Specify man pages to be installed
if(GZIP)
  add_custom_target(man-xdot ALL DEPENDS xdot.3.gz)
  add_custom_command(
    OUTPUT xdot.3.gz
    COMMAND ${GZIP} -9 --no-name --to-stdout xdot.3
      >"${CMAKE_CURRENT_BINARY_DIR}/xdot.3.gz"
    MAIN_DEPENDENCY xdot.3
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/xdot.3.gz
    DESTINATION ${MAN_INSTALL_DIR}/man3)
else()
  install(
    FILES xdot.3
    DESTINATION ${MAN_INSTALL_DIR}/man3
  )
endif()

# Specify library version and soversion
set_target_properties(xdot PROPERTIES
    VERSION 4.0.0
    SOVERSION 4
)
