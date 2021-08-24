BISON_TARGET(
  Exparse
  ${CMAKE_CURRENT_SOURCE_DIR}/exparse.y
  ${CMAKE_CURRENT_BINARY_DIR}/y.tab.c
  COMPILE_FLAGS "--yacc -Wno-yacc --defines --debug --verbose"
)

add_custom_command(
    OUTPUT exparse.c
    MAIN_DEPENDENCY y.tab.c
    COMMAND ${CMAKE_COMMAND}
    ARGS -P ${CMAKE_CURRENT_SOURCE_DIR}/exparse-replacements.cmake
)

add_library(expr STATIC
    # Header files
    exgram.h
    exlib.h
    exop.h
    exparse.h
    expr.h

    # Source files
    excc.c
    excontext.c
    exdata.c
    exerror.c
    exeval.c
    exexpr.c
    exopen.c
    extoken.c
    extype.c
    exzero.c
    exnospace.c
    exstash.c

    # Generated files
    ${CMAKE_CURRENT_BINARY_DIR}/exparse.c
)

target_include_directories(expr PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/.. # needed to find generated expr/y.tab.h
    ${GRAPHVIZ_LIB_DIR}/ast
    ${GRAPHVIZ_LIB_DIR}/vmalloc
    ${GRAPHVIZ_LIB_DIR}/sfio
    ${GRAPHVIZ_LIB_DIR}/cdt
)
