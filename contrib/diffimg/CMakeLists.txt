if(GD_LIBRARIES)

add_executable(diffimg diffimg.c)
target_include_directories(diffimg PRIVATE ${GD_INCLUDE_DIRS})
target_link_libraries(diffimg ${GD_LIBRARIES})

install(
  TARGETS diffimg
  RUNTIME DESTINATION ${BINARY_INSTALL_DIR})

install(
  FILES diffimg.1
  DESTINATION ${MAN_INSTALL_DIR})

endif()
