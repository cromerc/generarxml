AC_INIT([generarxml], [1.1], [chris@cromer.cl], [generarxml], [https://www.cromer.cl])
AC_PREFIX_DEFAULT([/usr])
AC_CONFIG_SRCDIR([src/main.c])
AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC([gcc])

AX_CHECK_COMPILE_FLAG([-std=c11], [
    CFLAGS+=" -std=c11"
], [
    echo "C compiler cannot compile C11 code"
    exit -1
])

AC_PROG_INSTALL

AC_PATH_PROG([M4], [m4])

AC_ARG_WITH(debug, [AS_HELP_STRING([--with-debug], [include debug messages])],
[case $with_debug in
    yes)    AC_DEFINE(DEBUG)
        ;;
    no)     ;;
    *)      AC_MSG_ERROR(["--with-debug does not take an argument."])
        ;;
esac])

AC_ARG_WITH(debug_encoding, [AS_HELP_STRING([--with-debug-encoding], [include encoding debug messages])],
[case $with_debug_encoding in
    yes)    AC_DEFINE(DEBUG_ENCODING)
        ;;
    no)     ;;
    *)      AC_MSG_ERROR(["--with-debug-encoding does not take an argument."])
        ;;
esac])

AC_DEFINE(_DEFAULT_SOURCE)

AM_PATH_XML2(2.9.0,,AC_MSG_ERROR([
*** Libxml is required to build generarxml; Available at
http://www.libxml.org/.]))

AM_CFLAGS=(`xml2-config --cflags --libs`)

PKG_CHECK_MODULES([libxml2], [libxml-2.0])

case "$host" in
*mingw*)
    AC_MSG_RESULT(OS Detected: $host)
    AC_MSG_RESULT(Adding iconv flag for mingw...)
    LDFLAGS="-liconv $AM_LDFLAGS $LDFLAGS"
    ;;
*)  ;;
esac

AC_CONFIG_FILES([Makefile
    src/Makefile
])

AC_OUTPUT
