# (C) 2016 Marvin Sielenkemper

AC_PREREQ([2.69])
AC_INIT([furnace], [0.1], [sielenk@gmx.de])
AM_INIT_AUTOMAKE()
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_CONFIG_MACRO_DIRS([m4 mysql-m4 boost-m4])

WITH_MYSQL()
MYSQL_USE_CLIENT_API()
MYSQL_SUBST()

# Checks for programs.
AC_PROG_CXX([g++])
AC_PROG_LIBTOOL()

# Checks for libraries.
BOOST_REQUIRE(1.55)
BOOST_FOREACH()
BOOST_SIGNALS2()

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_OUTPUT
