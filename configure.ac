# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([USTB-CLOCK], [v0.9], [https://github.com/jason23347/ustb-clock])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([subdir-objects])

# debug mode
AC_ARG_ENABLE(debug,
    AS_HELP_STRING([--enable-debug], [enable debugging, default: no]))
AS_IF([! test "x$enable_debug" = xyes ], [
    AC_DEFINE([NDEBUG], [1], [disable debugging code])
])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([ \
    arpa/inet.h \
    netinet/in.h \
    stddef.h \
    stdlib.h \
    string.h \
    sys/socket.h \
    sys/time.h \
    unistd.h \
])

# Checks for typedefs, structures, and compiler characteristics.

# POSIX types
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AX_PTHREAD()
AC_CHECK_FUNCS([gettimeofday memset strchr strerror])

LIBS="$LIBS -lpthread"

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
