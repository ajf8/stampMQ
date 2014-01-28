AC_PREREQ(2.63)

AC_INIT(stampmq, 0.1, ajf@eth0.org.uk)
AC_CONFIG_SRCDIR(src)
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

AC_PROG_CXX

AC_PROG_INSTALL
AC_PROG_LIBTOOL

BOOST_REQUIRE()
BOOST_ASIO()
BOOST_SYSTEM()
BOOST_THREADS()
BOOST_SIGNALSTWO()
BOOST_FILESYSTEM()
BOOST_REGEX()
BOOST_PROGRAM_OPTIONS()

PKG_CHECK_MODULES([LOG4CXX], [liblog4cxx])
PKG_CHECK_MODULES([YAMLCPP], [yaml-cpp])

AC_ARG_ENABLE(debug, [AS_HELP_STRING(--enable-debug,
                                     [enable debugging])])
if test x$enable_debug = xyes; then
    AC_DEFINE([DEBUG], [],  [enable debugging])
fi

AC_OUTPUT([Makefile
	   src/Makefile])
