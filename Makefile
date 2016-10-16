CC=gcc
BINDIR=out

DEBUG_FLAGS=-Wall -Werror

CFLAGS=$(DEBUG_FLAGS) -ggdb -Wconversion -std=c11 -D_DEFAULT_SOURCE $(shell pkg-config libxml-2.0 --cflags) -ansi
LDFLAGS=$(shell pkg-config libxml-2.0 --libs)

#CPPFLAGS
#	DEBUG:			show debug information during runtime
#	DEBUG_ENCODING:	show debug information related to encoding at runtime

CPPFLAGS=-DDEBUG
	
BINARY=generarxml

OBJS = \
	main.o \
	readconfig.o \
	readfile.o \
	encoding.o \
	string.o

all:	bin

bin:	$(OBJS)
	$(CC)$(xml2-config --cflags --libs) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(BINDIR)/$(BINARY)
	cp *.conf $(BINDIR)/
	cp *.dtd $(BINDIR)/
	cp Biblia.txt $(BINDIR)/

clean:
	rm -fR $(BINDIR)/* *.o

%.o:	%.c
	$(CC)$(xml2-config --cflags --libs) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -c $< -o $@
