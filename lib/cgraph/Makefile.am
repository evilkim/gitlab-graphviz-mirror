# $Id$ $Revision$
## Process this file with automake to produce Makefile.in

CGRAPH_VERSION="6:0:0"

pdfdir = $(pkgdatadir)/doc/pdf
pkgconfigdir = $(libdir)/pkgconfig

AM_CPPFLAGS = -I$(top_srcdir)/lib -I$(top_srcdir)/lib/cdt

pkginclude_HEADERS = cgraph.h
noinst_HEADERS = agxbuf.h cghdr.h strcasecmp.h
noinst_LTLIBRARIES = libcgraph_C.la
lib_LTLIBRARIES = libcgraph.la
pkgconfig_DATA = libcgraph.pc
man_MANS = cgraph.3
if ENABLE_MAN_PDFS
pdf_DATA = cgraph.3.pdf
endif

if ENABLE_MAN_PDFS
if HAVE_PS2PDF
pdf = $(pdf_DATA)
else
pdf = ERROR_NO_PS2PDF
endif
else
pdf =
endif

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

if ENABLE_MAN_PDFS
if HAVE_PS2PDF
cgraph.3.pdf: cgraph.3.ps
	@PS2PDF@ $<

cgraph.3.ps: $(srcdir)/cgraph.3
	@GROFF@ -Tps -man $< >$@
endif
endif

EXTRA_DIST = $(man_MANS) $(pdf) cmpnd.c \
	grammar.c grammar.h scan.c \
	cgraph.vcxproj*

DISTCLEANFILES = $(pdf) grammar.[ch] scan.c cgraph.3.ps
