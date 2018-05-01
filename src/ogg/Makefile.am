## Process this file with automake to produce Makefile.in
PFLAGS=-module -avoid-version -no-undefined

libsndogg_la_LDFLAGS = $(PFLAGS)
libsndogg_la_LIBADD = @COMPIZ_LIBS@
nodist_libsndogg_la_SOURCES = sndogg_options.c sndogg_options.h
dist_libsndogg_la_SOURCES = sndogg.c

BUILT_SOURCES = $(nodist_libsndogg_la_SOURCES)

AM_CPPFLAGS =                              \
	-I$(top_srcdir)/include             \
	@COMPIZ_CFLAGS@                     \
	-DDATADIR='"$(compdatadir)"'        \
	-DLIBDIR='"$(libdir)"'              \
	-DLOCALEDIR="\"@datadir@/locale\""  \
	-DIMAGEDIR='"$(imagedir)"'

moduledir = $(plugindir)

module_LTLIBRARIES = libsndogg.la

CLEANFILES = *_options.c *_options.h

sndogg_options.h: ../../metadata/sndogg.xml.in
		$(BCOP_BIN) --header $@ $<

sndogg_options.c: ../../metadata/sndogg.xml.in sndogg_options.h
		$(BCOP_BIN) --source $@ $<
