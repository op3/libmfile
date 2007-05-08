AC_PREREQ(2.59)

AC_INIT([src/mfile.h],[1.17],[ralf.schulze@ikp.uni-koeln.de])
AC_CONFIG_AUX_DIR(build-aux)
AC_CONFIG_SRCDIR([config.h.in])
AM_CONFIG_HEADER(config.h)

PACKAGE=libmfile

# release versioning
GENERIC_MAJOR_VERSION=1
GENERIC_MINOR_VERSION=0
GENERIC_MICRO_VERSION=0
GENERIC_VERSION=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION.$GENERIC_MICRO_VERSION
GENERIC_RELEASE=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION
AC_SUBST(GENERIC_RELEASE)
AC_SUBST(GENERIC_VERSION)

VERSION=$GENERIC_VERSION

# shared library versioning
GENERIC_LIBRARY_VERSION=1:1:0
#                       | | |
#                +------+ | +---+
#                |        |     |
#             current:revision:age
#                |        |     |
#                |        |     +- increment if interfaces have been added
#                |        |        set to zero if interfaces have been removed
#                                  or changed
#                |        +- increment if source code has changed
#                |           set to zero if current is incremented
#                +- increment if interfaces have been added, removed or changed

AC_SUBST(GENERIC_LIBRARY_VERSION)

AM_INIT_AUTOMAKE($PACKAGE, $VERSION, no-define)

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_LANG([C])
AC_PROG_LIBTOOL

AC_CONFIG_FILES([Makefile \
	         man/Makefile \
		 src/Makefile ])
AC_OUTPUT

