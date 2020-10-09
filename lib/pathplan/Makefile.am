# $Id$ $Revision$
## Process this file with automake to produce Makefile.in

PATHPLAN_VERSION="4:0:0"

pdfdir = $(pkgdatadir)/doc/pdf
pkgconfigdir = $(libdir)/pkgconfig

AM_CPPFLAGS = -I$(top_srcdir)/lib

if WITH_WIN32
AM_CPPFLAGS += -O0
endif

pkginclude_HEADERS = pathgeom.h pathplan.h
noinst_HEADERS = pathutil.h solvers.h tri.h vis.h vispath.h

noinst_LTLIBRARIES = libpathplan_C.la
lib_LTLIBRARIES = libpathplan.la
pkgconfig_DATA = libpathplan.pc
man_MANS = pathplan.3
if ENABLE_MAN_PDFS
pdf_DATA = pathplan.3.pdf

if HAVE_PS2PDF
pdf = $(pdf_DATA)
else
pdf = ERROR_NO_PS2PDF
endif
else
pdf =
endif

libpathplan_C_la_SOURCES = cvt.c inpoly.c route.c shortest.c \
	shortestpth.c solvers.c triang.c util.c visibility.c

libpathplan_la_LDFLAGS = -version-info $(PATHPLAN_VERSION) -no-undefined
libpathplan_la_SOURCES = $(libpathplan_C_la_SOURCES)
libpathplan_la_LIBADD = @MATH_LIBS@

if ENABLE_MAN_PDFS
if HAVE_PS2PDF
pathplan.3.pdf: pathplan.3.ps
	@PS2PDF@ $<

pathplan.3.ps: $(srcdir)/pathplan.3
	@GROFF@ -Tps -man $< >$@
endif
endif

EXTRA_DIST = $(man_MANS) $(pdf) path.lefty Pathplan.vcxproj* pathplan.def

DISTCLEANFILES = $(pdf) pathplan.3.ps
