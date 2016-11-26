# (C) 2016 Marvin Sielenkemper

AC_PREREQ([2.69])
AC_INIT([furnace], [0.1], [sielenk@gmx.de])
AM_INIT_AUTOMAKE()
AC_CONFIG_SRCDIR([main.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIRS([mysql-m4 boost-m4])

WITH_MYSQL()
MYSQL_USE_CLIENT_API()
MYSQL_SUBST()

# Checks for programs.
AC_PROG_CXX([g++])

# Checks for libraries.
AX_BOOST_BASE(1.55)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_OUTPUT
