ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

bin_PROGRAMS = generarxml
generarxml_SOURCES = main.c \
	main.h \
	readconfig.c \
	readconfig.h \
	readfile.c \
	readfile.h \
	makexml.c \
	makexml.h \
	encoding.c \
	encoding.h
generarxml_CPPFLAGS = $(XML_CPPFLAGS)
generarxml_LDFLAGS= $(XML_LIBS)
generarxml_LDADD = ${libxml2_LIBS} 
