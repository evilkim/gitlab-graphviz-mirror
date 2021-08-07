add_library(gvc++ SHARED
  GVContext.h
  GVContext.cpp
  GVLayout.h
  GVLayout.cpp
  GVRenderData.h
  GVRenderData.cpp
  )

set_target_properties(gvc++ PROPERTIES CXX_STANDARD 20)
set_target_properties(gvc++ PROPERTIES CXX_STANDARD_REQUIRED ON)

target_include_directories(gvc++ PRIVATE
  ${GRAPHVIZ_LIB_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GRAPHVIZ_LIB_DIR}/cdt
  ${GRAPHVIZ_LIB_DIR}/cgraph
  ${GRAPHVIZ_LIB_DIR}/common
  ${GRAPHVIZ_LIB_DIR}/gvc
  ${GRAPHVIZ_LIB_DIR}/pathplan
  )

target_link_libraries(gvc++ PUBLIC
  cgraph++
  gvc
  )

install(
  TARGETS gvc++
  RUNTIME DESTINATION ${BINARY_INSTALL_DIR}
  LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
  ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}
  )

install(
  FILES
  GVContext.h
  GVLayout.h
  GVRenderData.h
  DESTINATION ${HEADER_INSTALL_DIR}
  )

set_target_properties(gvc++ PROPERTIES
  VERSION 0.0.1
  SOVERSION 0
  )
