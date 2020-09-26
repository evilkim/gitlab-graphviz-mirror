# $Id$ $Revision$
## Process this file with automake to produce Makefile.in

pdfdir = $(pkgdatadir)/doc/pdf
#pkgconfigdir = $(libdir)/pkgconfig

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib \
        -I$(top_srcdir)/lib/common \
	-I$(top_srcdir)/lib/gvc \
	-I$(top_srcdir)/lib/neatogen \
	-I$(top_srcdir)/lib/pathplan \
	-I$(top_srcdir)/lib/cgraph \
	-I$(top_srcdir)/lib/cdt

if WITH_WIN32
AM_CFLAGS = -D_BLD_gvc=1
endif

pkginclude_HEADERS = pack.h
noinst_LTLIBRARIES = libpack_C.la
man_MANS = pack.3
pdf_DATA = pack.3.pdf
#pkgconfig_DATA = libpack.pc

if HAVE_PS2PDF
pdf = $(pdf_DATA)
else
pdf = ERROR_NO_PS2PDF
endif

libpack_C_la_SOURCES = ccomps.c pack.c

if HAVE_PS2PDF
pack.3.pdf: pack.3.ps
	@PS2PDF@ $<

pack.3.ps: $(srcdir)/pack.3
	@GROFF@ -Tps -man $< >$@
endif

EXTRA_DIST = $(man_MANS) $(pdf) ptest.c

DISTCLEANFILES = $(pdf_DATA) pack.3.ps

