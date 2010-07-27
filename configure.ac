AC_INIT([xmppconsole], [trunk], [dpc@ucore.info])
AM_INIT_AUTOMAKE

AC_PROG_CC
AC_PROG_RANLIB
AM_PROG_CC_C_O

AC_LANG([C])
AC_HEADER_STDC
AC_TYPE_SIZE_T

AC_MSG_CHECKING([whether to build with debug information])
AC_ARG_ENABLE([debug],
			  [AS_HELP_STRING([--enable-debug],
							  [enable debug data generation (def=no)])],
							  [debugit="$enableval"],
							  [debugit=no])
AC_MSG_RESULT([$debugit])

if test x"$debugit" = x"yes"; then
	AC_DEFINE([DEBUG],[],[Debug Mode])
	AM_CFLAGS="-g -Wall -Werror -O0 -pedantic -pedantic-errors "
else
	AC_DEFINE([NDEBUG],[],[No-debug Mode])
	AM_CFLAGS="-O3"
fi

PKG_CHECK_MODULES([LIBSTROPHE], [libstrophe])

AC_SUBST(LIBSTROPHE_NAME)
AC_SUBST(LIBSTROPHE_CFLAGS)
AC_SUBST(LIBSTROPHE_LIBS)
AC_SUBST(AM_CFLAGS)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
