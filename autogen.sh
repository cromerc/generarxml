#!/bin/sh

mkdir -p m4 \
&& aclocal \
&& automake --add-missing --copy \
&& autoconf \
&& rm -rf config.status config.log Makefile src/Makefile m4 autom4te.cache
