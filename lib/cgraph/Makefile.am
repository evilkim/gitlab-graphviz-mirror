# $Id$ $Revision$
## Process this file with automake to produce Makefile.in

CGRAPH_VERSION="6:0:0"

pdfdir = $(pkgdatadir)/doc/pdf
pkgconfigdir = $(libdir)/pkgconfig

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/lib/cdt

pkginclude_HEADERS = cgraph.h strcasecmp.h
noinst_HEADERS = agxbuf.h cghdr.h
noinst_LTLIBRARIES = libcgraph_C.la
lib_LTLIBRARIES = libcgraph.la
pkgconfig_DATA = libcgraph.pc
man_MANS = cgraph.3
pdf_DATA = cgraph.3.pdf

libcgraph_C_la_SOURCES = agerror.c agxbuf.c apply.c attr.c edge.c \
	flatten.c graph.c grammar.y id.c imap.c io.c mem.c node.c \
	obj.c pend.c rec.c refstr.c scan.l subg.c utils.c write.c

libcgraph_la_LDFLAGS = -version-info $(CGRAPH_VERSION) -no-undefined
libcgraph_la_SOURCES = $(libcgraph_C_la_SOURCES)
libcgraph_la_LIBADD = $(top_builddir)/lib/cdt/libcdt.la

scan.o scan.lo: scan.c grammar.h

scan.c: $(top_srcdir)/lib/cgraph/scan.l
	@LEX@ --outfile=$@ -i $(top_srcdir)/lib/cgraph/scan.l

grammar.c grammar.h: $(top_srcdir)/lib/cgraph/grammar.y
	@YACC@ -Wno-yacc -dv --output=grammar.c $(top_srcdir)/lib/cgraph/grammar.y

cgraph.3.pdf: cgraph.3.ps
	@PS2PDF@ $< $@

cgraph.3.ps: $(srcdir)/cgraph.3
	@GROFF@ -Tps -man $< >$@

EXTRA_DIST = $(man_MANS) $(pdf_DATA) cmpnd.c \
	grammar.c grammar.h scan.c \
	cgraph.vcxproj*

DISTCLEANFILES = $(pdf_DATA) grammar.[ch] scan.c cgraph.3.ps
