# $Id$Revision: 
## Process this file with automake to produce Makefile.in

pdfdir = $(pkgdatadir)/doc/pdf
pkgconfigdir = $(libdir)/pkgconfig
HOSTCC ?= $(CC)
GVPR_VERSION="2:0:0"

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib \
	-I$(top_srcdir)/lib/common \
	-I$(top_builddir)/lib/common \
	-I$(top_srcdir)/lib/cgraph \
	-I$(top_srcdir)/lib/cdt \
	-DDFLT_GVPRPATH="\".$(PATH_SEPARATOR)$(pkgdatadir)/gvpr\""

pkginclude_HEADERS = gvpr.h
noinst_HEADERS = actions.h compile.h gprstate.h parse.h queue.h trieFA.h trie.c
noinst_LTLIBRARIES = libgvpr_C.la
lib_LTLIBRARIES = libgvpr.la
pkgconfig_DATA = libgvpr.pc
man_MANS = gvpr.3
if ENABLE_MAN_PDFS
pdf_DATA = gvpr.3.pdf
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

libgvpr_C_la_SOURCES = actions.c compile.c gvpr.c gprstate.c parse.c queue.c 
$(libgvpr_C_la_OBJECTS): gdefs.h
libgvpr_C_la_DEPENDENCIES = \
	$(top_builddir)/lib/expr/libexpr_C.la \
	$(top_builddir)/lib/common/libcommon_C.la \
	$(top_builddir)/lib/ingraphs/libingraphs_C.la

if ENABLE_MAN_PDFS
if HAVE_PS2PDF
gvpr.3.pdf: gvpr.3.ps
	@PS2PDF@ $<

gvpr.3.ps: $(srcdir)/gvpr.3
	@GROFF@ -Tps -man $< >$@
endif
endif

libgvpr_la_LDFLAGS = -version-info $(GVPR_VERSION) -no-undefined
libgvpr_la_SOURCES = $(libgvpr_C_la_SOURCES)
libgvpr_la_LIBADD = $(libgvpr_C_la_LIBADD) \
	$(top_builddir)/lib/ingraphs/libingraphs_C.la \
	$(top_builddir)/lib/expr/libexpr_C.la \
	$(top_builddir)/lib/cdt/libcdt.la \
	$(top_builddir)/lib/cgraph/libcgraph.la \
	$(MATH_LIBS)
libgvpr_la_DEPENDENCIES = $(libgvpr_C_la_DEPENDENCIES)

if WITH_WIN32
libgvpr_la_LDFLAGS += -export-symbols-regex gvpr
endif

mkdefs$(EXEEXT):
	$(HOSTCC) $(srcdir)/mkdefs.c -o mkdefs$(EXEEXT)

gdefs.h: $(top_srcdir)/lib/gvpr/gprdata mkdefs$(EXEEXT)
	./mkdefs$(EXEEXT) gdefs.h < $(top_srcdir)/lib/gvpr/gprdata

EXTRA_DIST = $(man_MANS) $(pdf) gprdata mkdefs.c gdefs.h trie.c gvprlib.vcxproj*

DISTCLEANFILES = $(pdf) gdefs.h mkdefs$(EXEEXT) gvpr.3.ps
