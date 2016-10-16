AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = generarxml
generarxml_SOURCES = src/main.c \
	src/main.h \
	src/readconfig.c \
	src/readconfig.h \
	src/readfile.c \
	src/readfile.h \
	src/encoding.c \
	src/encoding.h \
	src/string.c \
	src/string.h
generarxml_CPPFLAGS = $(XML_CPPFLAGS)
generarxml_LDFLAGS= $(XML_LIBS)
EXTRA_DIST = misc/Biblia.txt \
	misc/config.dtd \
	misc/chris.conf \
	src/misc/Biblia.txt \
	src/misc/config.dtd \
	src/misc/chris.conf
