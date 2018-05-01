## Process this file with automake to produce Makefile.in
PFLAGS=-module -avoid-version -no-undefined

libsndportaudio_la_LDFLAGS = $(PFLAGS)
libsndportaudio_la_LIBADD = @COMPIZ_LIBS@
nodist_libsndportaudio_la_SOURCES = sndogg_options.c sndogg_options.h
dist_libsndportaudio_la_SOURCES = sndportaudio.c

BUILT_SOURCES = $(nodist_libsndportaudio_la_SOURCES)

AM_CPPFLAGS =                              \
	-I$(top_srcdir)/include             \
	@COMPIZ_CFLAGS@                     \
	-DDATADIR='"$(compdatadir)"'        \
	-DLIBDIR='"$(libdir)"'              \
	-DLOCALEDIR="\"@datadir@/locale\""  \
	-DIMAGEDIR='"$(imagedir)"'

moduledir = $(plugindir)

module_LTLIBRARIES = libsndportaudio.la

CLEANFILES = *_options.c *_options.h

sndportaudio_options.h: ../../metadata/sndogg.xml.in
		$(BCOP_BIN) --header $@ $<

sndportaudio_options.c: ../../metadata/sndportaudio.xml.in sndportaudio_options.h
		$(BCOP_BIN) --source $@ $<
