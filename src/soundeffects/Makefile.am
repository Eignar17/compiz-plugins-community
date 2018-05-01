## Process this file with automake to produce Makefile.in
PFLAGS=-module -avoid-version -no-undefined

libsoundeffects_la_LDFLAGS = $(PFLAGS)
libsoundeffects_la_LIBADD = @COMPIZ_LIBS@
nodist_libsoundeffects_la_SOURCES = soundeffects_options.c soundeffects_options.h
dist_libsoundeffects_la_SOURCES = soundeffects.c

BUILT_SOURCES = $(nodist_libsoundeffects_la_SOURCES)

AM_CPPFLAGS =                              \
	-I$(top_srcdir)/include             \
	@COMPIZ_CFLAGS@                     \
	-DDATADIR='"$(compdatadir)"'        \
	-DLIBDIR='"$(libdir)"'              \
	-DLOCALEDIR="\"@datadir@/locale\""  \
	-DIMAGEDIR='"$(imagedir)"'

moduledir = $(plugindir)

module_LTLIBRARIES = libsoundeffects.la

CLEANFILES = *_options.c *_options.h

soundeffects_options.h: ../../metadata/soundeffects.xml.in
		$(BCOP_BIN) --header $@ $<

soundeffects_options.c: ../../metadata/soundeffects.xml.in soundeffects_options.h
		$(BCOP_BIN) --source $@ $<
