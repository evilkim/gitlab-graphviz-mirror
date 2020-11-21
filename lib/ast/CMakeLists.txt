add_library(ast STATIC
    # Header files
    ast.h
    error.h
    hashkey.h

    # Source files
    pathpath.c
    sfstr.h
    chresc.c
    chrtoi.c
    error.c
    fmtbuf.c
    fmtesc.c
    pathaccess.c
    pathbin.c
    pathcanon.c
    pathcat.c
    pathexists.c
    pathfind.c
    pathgetlink.c
    strcopy.c
    stresc.c
    strmatch.c
)

target_include_directories(ast PRIVATE
    ${GRAPHVIZ_LIB_DIR}
)
