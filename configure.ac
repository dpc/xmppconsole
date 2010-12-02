# http://www.freesoftwaremagazine.com/books/autotools_a_guide_to_autoconf_automake_libtool

AC_INIT([xmppconsole], [0.1.dev], [dpc@ucore.info])
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([src/main.c])

AC_CONFIG_HEADERS([config.h])

AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_CC_C_O
AC_HEADER_STDC

PKG_CHECK_MODULES([LIBSTROPHE], [libstrophe])
PKG_CHECK_MODULES([LIBAPR], [apr-1])

AC_CHECK_LIB(
	[readline], [readline],
	[],
	[AC_MSG_ERROR AC_MSG_ERROR(readline library not found)]
)
AC_CHECK_HEADER(
	[readline/readline.h], [],
	[AC_MSG_ERROR(readline library headers not found)]
)

AC_SUBST([LIBSTROPHE_NAME])
AC_SUBST([LIBSTROPHE_CFLAGS])
AC_SUBST([LIBSTROPHE_LIBS])
AC_SUBST([LIBAPR_NAME])
AC_SUBST([LIBAPR_CFLAGS])
AC_SUBST([LIBAPR_LIBS])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
