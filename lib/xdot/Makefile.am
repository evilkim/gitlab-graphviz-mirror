# $Id$ $Revision$
# ## Process this file with automake to produce Makefile.in

pkgconfigdir = $(libdir)/pkgconfig

XDOT_VERSION="4:0:0"

pdfdir = $(pkgdatadir)/doc/pdf
pkginclude_HEADERS = xdot.h

AM_CPPFLAGS = -I$(top_srcdir)/lib

noinst_LTLIBRARIES = libxdot_C.la
lib_LTLIBRARIES = libxdot.la
pkgconfig_DATA = libxdot.pc

man_MANS = xdot.3
pdf_DATA = xdot.3.pdf

if HAVE_PS2PDF
pdf = $(pdf_DATA)
else
pdf = ERROR_NO_PS2PDF
endif

libxdot_C_la_SOURCES = xdot.c
libxdot_la_LDFLAGS = -version-info $(XDOT_VERSION) -no-undefined
libxdot_la_SOURCES = $(libxdot_C_la_SOURCES)

if HAVE_PS2PDF
xdot.3.pdf: xdot.3.ps
	@PS2PDF@ $< $@

xdot.3.ps: $(srcdir)/xdot.3
	@GROFF@ -Tps -man $< >$@
endif

EXTRA_DIST = $(man_MANS) $(pdf) xdot.vcxproj*

DISTCLEANFILES = $(pdf_DATA) xdot.3.ps

