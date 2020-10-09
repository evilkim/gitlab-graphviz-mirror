# $Id$ $Revision$
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib \
	-I$(top_srcdir)/lib/cgraph \
	-I$(top_srcdir)/lib/cdt

noinst_HEADERS = spinehdr.h quad.h spine.h subset.h union_find.h
noinst_LTLIBRARIES = libspine_C.la

libspine_C_la_SOURCES = quad.c spine.c subset.c union_find.c

EXTRA_DIST = $(man_MANS)
