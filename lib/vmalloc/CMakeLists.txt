add_library(vmalloc STATIC
    # Header files
    vmalloc.h
    vmhdr.h

    # Source files
    vmalloc.c
    vmclear.c
    vmclose.c
    vmopen.c
    vmstrdup.c
)

target_include_directories(vmalloc PRIVATE
    ${GRAPHVIZ_LIB_DIR}
)
