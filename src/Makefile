# Este makefile es el antiguo y mas limitado, no puede detectar el entorno para verificar que la compilación va a funcionar

CC=gcc
BINDIR=out
SRCDIR=.
EXTRADIR=misc
DEBUG_FLAGS=-Wall -Werror

CFLAGS=$(DEBUG_FLAGS) -ggdb -Wconversion -std=c11 -D_DEFAULT_SOURCE $(shell pkg-config libxml-2.0 --cflags) -ansi
LDFLAGS=$(shell pkg-config libxml-2.0 --libs)

#CPPFLAGS
#	DEBUG:			show debug information during runtime
#	DEBUG_ENCODING:	show debug information related to encoding at runtime

CPPFLAGS=-DDEBUG
	
BINARY=generarxml

OBJS = \
	$(SRCDIR)/main.o \
	$(SRCDIR)/readconfig.o \
	$(SRCDIR)/readfile.o \
	$(SRCDIR)/encoding.o \
	$(SRCDIR)/string.o

all:	bin

bin:	$(OBJS)
	[ -d "out" ] && echo ¨out already exists¨ || mkdir -v out
	$(CC)$(xml2-config --cflags --libs) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(BINDIR)/$(BINARY)
	cp $(EXTRADIR)/*.conf $(BINDIR)/
	cp $(EXTRADIR)/*.dtd $(BINDIR)/
	cp $(EXTRADIR)/*.txt $(BINDIR)/

clean:
	rm -fR $(BINDIR) $(SRCDIR)/*.o

%.o:	%.c
	$(CC)$(xml2-config --cflags --libs) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -c $< -o $@
