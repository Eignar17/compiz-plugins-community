## Process this file with automake to produce Makefile.in
PFLAGS=-module -avoid-version -no-undefined

libsound_la_LDFLAGS = $(PFLAGS)
libsound_la_LIBADD = @COMPIZ_LIBS@
nodist_libsound_la_SOURCES = sound_options.c sound_options.h
dist_libsound_la_SOURCES = sound.c

BUILT_SOURCES = $(nodist_libsound_la_SOURCES)

AM_CPPFLAGS =                              \
	-I$(top_srcdir)/include             \
	@COMPIZ_CFLAGS@                     \
	-DDATADIR='"$(compdatadir)"'        \
	-DLIBDIR='"$(libdir)"'              \
	-DLOCALEDIR="\"@datadir@/locale\""  \
	-DIMAGEDIR='"$(imagedir)"'

moduledir = $(plugindir)

module_LTLIBRARIES = libsound.la

CLEANFILES = *_options.c *_options.h

sound_options.h: ../../metadata/sound.xml.in
		$(BCOP_BIN) --header $@ $<

sound_options.c: ../../metadata/sound.xml.in sound_options.h
		$(BCOP_BIN) --source $@ $<
