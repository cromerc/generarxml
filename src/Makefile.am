ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

bin_PROGRAMS = generarxml
generarxml_SOURCES = main.c \
	main.h \
	readconfig.c \
	readconfig.h \
	readfile.c \
	readfile.h \
	encoding.c \
	encoding.h
generarxml_CPPFLAGS = $(XML_CPPFLAGS)
generarxml_LDFLAGS= $(XML_LIBS)
generarxml_LDADD = ${libxml2_LIBS} 
EXTRA_DIST = misc/Biblia.txt \
	misc/config.dtd \
	misc/chris.conf \
	src/misc/Biblia.txt \
	src/misc/config.dtd \
	src/misc/chris.conf
