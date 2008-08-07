AC_PREREQ(2.59)

AC_INIT([src/mfile.h],[1.18],[ralf.schulze@ikp.uni-koeln.de])
AC_CONFIG_AUX_DIR(build-aux)
AC_CONFIG_SRCDIR([src/config.h.in])
AM_CONFIG_HEADER(src/config.h)

PACKAGE=libmfile

# release versioning
GENERIC_MAJOR_VERSION=1
GENERIC_MINOR_VERSION=0
GENERIC_MICRO_VERSION=4
GENERIC_VERSION=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION.$GENERIC_MICRO_VERSION
GENERIC_RELEASE=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION
AC_SUBST(GENERIC_RELEASE)
AC_SUBST(GENERIC_VERSION)

VERSION=$GENERIC_VERSION

# shared library versioning
GENERIC_LIBRARY_VERSION=1:4:0
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

## Check for some compiler features
#
AC_PROG_CC_STDC # Check for ISO Standard C support (currently C99)
#if test x"${ac_cv_prog_cc_stdc}" != x"no"; then
#       AC_DEFINE([...], [1], [...])
#fi

# Check for __attribute__((visibility("hidden"))) support
AC_TRY_COMPILE([], [void __attribute__ ((visibility("hidden"))) foo(void) { retu
rn 1; };],
                attribute_visibility=yes,
                attribute_visibility=no)

if test x"$attribute_visibility" == x"yes"; then
        AC_DEFINE([HAVE_ATTRIBUTE_VISIBILITY], 1, [Define if compiler supports visibility attribute])
        AC_SUBST([FVISIBILITY], ["-fvisibility=hidden"])
fi

AC_C_CONST      # Check for 'const' and set to empty if not supported 

# Check for endianness 
AC_C_BIGENDIAN(AC_DEFINE([LOWENDIAN], [0], [Define on a low-endian system]), 
				AC_DEFINE([LOWENDIAN], [1], [Define on a low-endian system]))

# Check unit testing (see http://check.sourceforgne.net)
#PKG_CHECK_MODULES([CHECK], [check >= 0.9.3],,AC_MSG_WARN([Cannot find "check" unit testing framework]))
AM_PROG_CC_C_O

###
## Add the option to enable libefence for memory debugging
###
AC_ARG_WITH([efence],
            [AS_HELP_STRING([--with-efence],
              [enable libefence (for memory debugging) @<:@default=no@:>@])])
AS_IF([test "x$with_efence" == xyes],
        [AC_CHECK_LIB([efence], [main], [],
           [if test "x$with_efence" == xyes; then
              AC_MSG_FAILURE(
                   [--with-efence was given, but test for libefence failed])
               fi
              ])])

###
## Enable debugging output
###
AC_ARG_WITH([debug],
            [AS_HELP_STRING([--with-debug-output],
              [enable debugging output @<:@default=no@:>@])])
AS_IF([test "x$with_debug_output" == xyes],
	    AC_DEFINE([DEBUG_OUTPUT], [1],[Enable debugging output])
)		

AC_CONFIG_FILES([Makefile \
	         man/Makefile \
		     src/Makefile test/Makefile ])
AC_OUTPUT

