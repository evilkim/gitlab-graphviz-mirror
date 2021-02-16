## Process this file with automake to produce Makefile.in

SUBDIRS = Sfio_f

AM_CPPFLAGS = -I$(top_srcdir)/lib

noinst_HEADERS = sfhdr.h sfio.h sfio_t.h
noinst_LTLIBRARIES = libsfio_C.la

libsfio_C_la_SOURCES = sfclose.c sfcvt.c sfdisc.c \
	sfexcept.c sfextern.c sffilbuf.c \
	sfflsbuf.c \
	sfmode.c sfnew.c \
	sfnputc.c sfopen.c sfpkrd.c \
	sfprintf.c \
	sfputr.c sfraise.c sfrd.c sfread.c \
	sfresize.c sfscanf.c sfseek.c sfsetbuf.c sfsetfd.c \
	sfsk.c sfstack.c sfswap.c sfsync.c \
	sftable.c sftmp.c sfungetc.c sfvprintf.c \
	sfvscanf.c sfwr.c sfwrite.c

libsfio_C_la_DEPENDENCIES = \
	Sfio_f/libsfiof_C.la

EXTRA_DIST = README sfio.vcxproj*
