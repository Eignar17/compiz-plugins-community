## Process this file with automake to produce Makefile.in
PFLAGS=-module -avoid-version -no-undefined

libheadtracking_la_LDFLAGS = $(PFLAGS)
libheadtracking_la_LIBADD = @COMPIZ_LIBS@
nodist_libheadtracking_la_SOURCES = headtracking_options.c headtracking_options.h
dist_libheadtracking_la_SOURCES = facedetect.c headtracking.c facedetect.h compiz-wiimote.h

BUILT_SOURCES = $(nodist_libheadtracking_la_SOURCES)

AM_CPPFLAGS =                              \
	-I$(top_srcdir)/include             \
	@COMPIZ_CFLAGS@                     \
	-DDATADIR='"$(compdatadir)"'        \
	-DLIBDIR='"$(libdir)"'              \
	-DLOCALEDIR="\"@datadir@/locale\""  \
	-DIMAGEDIR='"$(imagedir)"'          \
	-std=c99

moduledir = $(plugindir)

module_LTLIBRARIES = libheadtracking.la

CLEANFILES = *_options.c *_options.h

headtracking_options.h: ../../metadata/headtracking.xml.in
		$(BCOP_BIN) --header $@ $<

headtracking_options.c: ../../metadata/headtracking.xml.in headtracking_options.h
		$(BCOP_BIN) --source $@ $<
