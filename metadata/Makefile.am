xmldir       = $(metadatadir)

xml_in_files =           \
	boing.xml.in         \
	dclick.xml.in        \
	flash.xml.in         \
	headtracking.xml.in  \
	lazypointer.xml.in   \
	maze.xml.in          \
	mousegestures.xml.in \
	newton.xml.in        \
	peek.xml.in          \
	prompt.xml.in        \
	roach.xml.in         \
	rubik.xml.in         \
	sndogg.xml.in        \
	sndportaudio.xml.in  \
	sound.xml.in         \
	soundeffects.xml.in  \
        visualevent.xml.in   \
	wiimote.xml.in       \
	wiitrack.xml.in

xml_files    = $(xml_in_files:.xml.in=.xml)
xml_DATA     = $(xml_files)

@INTLTOOL_XML_RULE@

EXTRA_DIST =		\
	$(xml_in_files)

DISTCLEANFILES =	\
	$(xml_files)
	
	
