AC_INIT([generarxml], [1.0], [chris@cromer.cl])
AC_PREFIX_DEFAULT([/usr])
AC_CONFIG_SRCDIR([src/main.c])
AM_INIT_AUTOMAKE([subdir-objects foreign])
AC_PROG_CC

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
    yes)	AC_DEFINE(DEBUG)
		;;
    no)		;;
    *)		AC_MSG_ERROR(["--with-debug does not take an argument."])
		;;
esac])

AC_ARG_WITH(debug_encoding, [AS_HELP_STRING([--with-debug-encoding], [include encoding debug messages])],
[case $with_debug_encoding in
    yes)	AC_DEFINE(DEBUG_ENCODING)
		;;
    no)		;;
    *)		AC_MSG_ERROR(["--with-debug-encoding does not take an argument."])
		;;
esac])

AC_DEFINE(_DEFAULT_SOURCE)

AM_PATH_XML2(2.9.0,,AC_MSG_ERROR([
*** Libxml is required to build generarxml; Available at
http://www.libxml.org/.]))

AM_CFLAGS=(`xml2-config --cflags`)

PKG_CHECK_MODULES(LIBXML2, libxml-2.0)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
