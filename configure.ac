AC_PREREQ(2.68)
AC_INIT([xwiichvt],
        0.1,
        [https://github.com/anduchs/xwiichvt],
        xwiichvt)

AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE([dist-bzip2 -Wall -Werror 1.11 foreign no-dist-gzip tar-pax])

AC_PROG_CC

AC_ARG_VAR([RONN], [path to the ronn])
AC_PATH_PROG([RONN], [ronn], [#])

PKG_CHECK_MODULES(REQS, libudev libxwiimote)
AC_SUBST(REQS_CFLAGS)
AC_SUBST(REQS_LIBS)

AC_OUTPUT([Makefile])
