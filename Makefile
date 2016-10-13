CC=gcc
BINDIR=out

DEBUG_FLAGS=-Wall -Werror

CFLAGS=$(DEBUG_FLAGS) -ggdb $(shell pkg-config libxml-2.0 --cflags) -ansi -I ./headers
LDFLAGS=$(shell pkg-config libxml-2.0 --libs)
	
BINARY=generarxml

OBJS = \
	main.o \
	readconfig.o

all:	bin

bin:	$(OBJS)
	$(CC) $(xml2-config --cflags --libs) $(CFLAGS) $(OBJS) $(LDFLAGS) \
	-o $(BINDIR)/$(BINARY)

clean:
	rm -fR $(BINDIR)/$(BINARY) *.o

%.o:	%.c
	$(CC) $(xml2-config --cflags --libs) $(CFLAGS) -c $< -o $@ $(LDFLAGS)
