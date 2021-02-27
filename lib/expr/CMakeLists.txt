BISON_TARGET(
  Exparse
  exparse.y
  ${CMAKE_CURRENT_BINARY_DIR}/y.tab.c
  COMPILE_FLAGS "--yacc -Wno-yacc --defines --debug --verbose"
)

add_custom_command(
    OUTPUT exparse.c
    MAIN_DEPENDENCY y.tab.c
    COMMAND ${CMAKE_COMMAND}
    ARGS -P ${CMAKE_CURRENT_SOURCE_DIR}/exparse-replacements.cmake
)

if (WIN32)
    add_custom_command(
        OUTPUT exparse.h
        MAIN_DEPENDENCY y.tab.h
        COMMAND echo \#ifndef _EXPARSE_H > exparse.h
        COMMAND echo \#define _EXPARSE_H >> exparse.h
        COMMAND type y.tab.h >> exparse.h
        COMMAND echo \#endif /* _EXPARSE_H */ >> exparse.h
    )
else()
    add_custom_command(
        OUTPUT exparse.h
        MAIN_DEPENDENCY y.tab.h
        COMMAND echo "\\#ifndef _EXPARSE_H" > exparse.h
        COMMAND echo "\\#define _EXPARSE_H" >> exparse.h
        COMMAND cat y.tab.h >> exparse.h
        COMMAND echo "\\#endif /* _EXPARSE_H */" >> exparse.h
    )
endif()

add_library(expr STATIC
    # Header files
    exgram.h
    exlib.h
    exop.h
    expr.h

    # Source files
    excc.c
    excontext.c
    exdata.c
    exerror.c
    exeval.c
    exexpr.c
    exlexname.c
    exopen.c
    exrewind.c
    extoken.c
    extype.c
    exzero.c
    exnospace.c
    exstash.c

    # Generated files
    ${CMAKE_CURRENT_BINARY_DIR}/exparse.h
    ${CMAKE_CURRENT_BINARY_DIR}/exparse.c
)

target_include_directories(expr PRIVATE
    ${GRAPHVIZ_LIB_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/.. # needed to find generated expr/exparse.h
    ${GRAPHVIZ_LIB_DIR}/ast
    ${GRAPHVIZ_LIB_DIR}/vmalloc
    ${GRAPHVIZ_LIB_DIR}/sfio
    ${GRAPHVIZ_LIB_DIR}/cdt
)
