## Process this file with automake to produce Makefile.in

EXPR_VERSION="4:0:0"

pdfdir = $(pkgdatadir)/doc/pdf
pkgconfigdir = $(libdir)/pkgconfig

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib \
	-I$(top_srcdir)/lib/cdt

noinst_HEADERS = exgram.h exlib.h exop.h expr.h
noinst_LTLIBRARIES = libexpr_C.la

### expr.h includes ast.h and vmalloc.h which we don't install
#lib_LTLIBRARIES = libexpr.la
man_MANS = expr.3
if ENABLE_MAN_PDFS
pdf_DATA = expr.3.pdf

if HAVE_PS2PDF
pdf = $(pdf_DATA)
else
pdf = ERROR_NO_PS2PDF
endif
else
pdf =
endif

#### -no-undefined breaks Mac OS/X builds.
#### libast has "extern char **environ" which remains
####      undefined until linked with main()
#libexpr_la_LDFLAGS = -version-info $(EXPR_VERSION) -no-undefined

libexpr_C_la_SOURCES = excc.c excontext.c exdata.c exerror.c \
	exeval.c exexpr.c exlexname.c exopen.c exrewind.c extoken.c \
	extype.c exzero.c exparse.y exnospace.c exstash.c 
libexpr_C_la_LIBADD = \
	$(top_builddir)/lib/ast/libast_C.la \
	$(top_builddir)/lib/vmalloc/libvmalloc_C.la \
	$(top_builddir)/lib/sfio/libsfio_C.la \
	$(top_builddir)/lib/cdt/libcdt.la

#libexpr_la_LDFLAGS = -version-info $(EXPR_VERSION)
#libexpr_la_SOURCES = $(libexpr_C_la_SOURCES)
#libexpr_la_LIBADD = $(libexpr_C_la_LIBADD)

$(libexpr_C_la_OBJECTS) $(libexpr_la_OBJECTS): \
	exparse.h exparse.c exgram.h exop.h

exparse.c: y.tab.c 
	@SED@ "s/fprintf/sfprintf/g" <y.tab.c | \
		@SED@ "s/FILE/Sfio_t/g" | \
		@SED@ "s/stderr/sfstderr/g" > exparse.c

exparse.h: y.tab.h
	echo "#ifndef _EXPARSE_H" > exparse.h
	echo "#define _EXPARSE_H" >> exparse.h
	cat y.tab.h >> exparse.h
	echo "#endif /* _EXPARSE_H */" >> exparse.h

y.tab.c y.tab.h: y.output

y.output: $(top_srcdir)/lib/expr/exparse.y
	@YACC@ -Wno-yacc -dtv $(top_srcdir)/lib/expr/exparse.y -o y.tab.c

if ENABLE_MAN_PDFS
if HAVE_PS2PDF
expr.3.pdf: expr.3.ps
	@PS2PDF@ $<

expr.3.ps: $(srcdir)/expr.3
	@GROFF@ -Tps -man $< >$@
endif
endif

EXTRA_DIST = $(man_MANS) $(pdf) \
	Makefile.nmake Makefile.orig \
	exparse.c exparse.h \
	y.tab.c y.tab.h y.output expr.vcxproj*

DISTCLEANFILES = $(pdf) exparse.[ch] y.tab.[ch] y.output expr.3.ps
