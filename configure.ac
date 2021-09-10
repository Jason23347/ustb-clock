# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([USTB-CLOCK], [v0.9], [https://github.com/jason23347/ustb-clock])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([subdir-objects])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# POSIX types
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
# AX_PTHREAD()

LIBS="$LIBS -lfiber -ldl -lpthread"

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
