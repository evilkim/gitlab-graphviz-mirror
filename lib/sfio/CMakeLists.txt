add_library(sfio STATIC
    # Header files
    sfhdr.h
    sfio.h
    sfio_t.h

    # Source files
    sfclose.c
    sfcvt.c
    sfdisc.c
    sfexcept.c
    sfextern.c
    sffilbuf.c
    sfflsbuf.c
    sfmode.c
    sfnew.c
    sfnputc.c
    sfopen.c
    sfpkrd.c
    sfprintf.c
    sfputr.c
    sfraise.c
    sfrd.c
    sfread.c
    sfresize.c
    sfscanf.c
    sfseek.c
    sfsetbuf.c
    sfsetfd.c
    sfsk.c
    sfstack.c
    sfswap.c
    sfsync.c
    sftable.c
    sftmp.c
    sfungetc.c
    sfvprintf.c
    sfvscanf.c
    sfwr.c
    sfwrite.c
)

target_include_directories(sfio PRIVATE
    ${GRAPHVIZ_LIB_DIR}
)

