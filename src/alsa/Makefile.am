## Process this file with automake to produce Makefile.in
PFLAGS=-module -avoid-version -no-undefined

libalsa_la_LDFLAGS = $(PFLAGS)
libalsa_la_LIBADD = @COMPIZ_LIBS@
nodist_libalsa_la_SOURCES = alsa_options.c alsa_options.h
dist_libalsa_la_SOURCES = alsa.c

BUILT_SOURCES = $(nodist_libalsa_la_SOURCES)

AM_CPPFLAGS =                              \
	-I$(top_srcdir)/include             \
	@COMPIZ_CFLAGS@                     \
	-DDATADIR='"$(compdatadir)"'        \
	-DLIBDIR='"$(libdir)"'              \
	-DLOCALEDIR="\"@datadir@/locale\""  \
	-DIMAGEDIR='"$(imagedir)"'

moduledir = $(plugindir)

module_LTLIBRARIES = libalsa.la

CLEANFILES = *_options.c *_options.h

alsa_options.h: ../../metadata/alsa.xml.in
		$(BCOP_BIN) --header $@ $<

alsa_options.c: ../../metadata/alsa.xml.in alsa_options.h
		$(BCOP_BIN) --source $@ $<
