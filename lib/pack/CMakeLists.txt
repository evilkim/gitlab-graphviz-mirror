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
if(GZIP)
  add_custom_target(man-pack ALL DEPENDS pack.3.gz)
  add_custom_command(
    OUTPUT pack.3.gz
    COMMAND ${GZIP} -9 --no-name --to-stdout pack.3
      >"${CMAKE_CURRENT_BINARY_DIR}/pack.3.gz"
    MAIN_DEPENDENCY pack.3
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/pack.3.gz
    DESTINATION ${MAN_INSTALL_DIR}/man3)
else()
  install(
    FILES pack.3
    DESTINATION ${MAN_INSTALL_DIR}/man3
  )
endif()
