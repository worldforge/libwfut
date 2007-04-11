AC_INIT(libwfut, 0.1.0, simon@worldforge.org)

AC_CONFIG_SRCDIR([libwfut/WFUT.cpp])
AC_CONFIG_HEADERS([config.h])

AC_CANONICAL_HOST

AM_INIT_AUTOMAKE()

LIBWFUT_CURRENT=0
LIBWFUT_REVISION=1
LIBWFUT_AGE=0

AC_PROG_CXX
AC_PROG_LIBTOOL

CXXFLAGS="$CXXFLAGS -Wall -DTIXML_USE_STL=1"

AC_LANG_PUSH(C++)

PKG_CHECK_MODULES(SIGC, sigc++-2.0 != 2.0.8,
    [
        CXXFLAGS="$CXXFLAGS $SIGC_CFLAGS"
        LDFLAGS="$LDFLAGS $SIGC_LIBS"
        LIBWFUT_REQUIRES="$LIBWFUT_REQUIRES sigc++-2.0"
    ],
    [
        AC_MSG_ERROR([Couldn't find sigc++ library, or an invalid version was found. Please see http://libsigc.sourceforge.net/ and obtain sigc++ 2.0.])
    ]
)

CURL_VERSION=7.15.0
PKG_CHECK_MODULES(CURL, libcurl >= $CURL_VERSION,
    [
        CXXFLAGS="$CXXFLAGS $CURL_CFLAGS"
        LDFLAGS="$LDFLAGS $CURL_LIBS"
        LIBWFUT_REQUIRES="$LIBWFUT_REQUIRES libcurl"
    ],
    [
        AC_MSG_ERROR([Couldn't find curl library. Please see http://curl.haxx.se/ and obtain a curl version greater than $CURL_VERSION])
    ]
)


AC_LANG_POP(C++)

AC_CHECK_LIB(z, crc32, [], [AC_MSG_ERROR([Unable to find zlib])])

AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)

LIBWFUT_LIB_SUFFIX="-0.1"
LIBWFUT_LIBS="-lwfut$LIBWFUT_LIB_SUFFIX"
LIBWFUT_VERSION_INFO=$LIBWFUT_CURRENT:$LIBWFUT_REVISION:$LIBWFUT_AGE

AC_SUBST(LIBWFUT_LIB_SUFFIX)
AC_SUBST(LIBWFUT_LIBS)
AC_SUBST(LIBWFUT_REQUIRES)
AC_SUBST(LIBWFUT_VERSION_INFO)

if test "$enable_shared" = yes; then
  if test "x$CYGWIN" = xyes -o "x$MINGW32" = xyes; then
    LIBWFUT_DLL=libwfut$LIBWFUT_LIB_SUFFIX.dll
    AC_SUBST(LIBWFUT_DLL)
    AC_CHECK_TOOL(DLLWRAP, dllwrap)
    AC_CHECK_TOOL(DLLTOOL, dlltool)
    BUILD_IT=true
  else
    BUILD_IT=false
  fi
else
  BUILD_IT=false
fi

AM_CONDITIONAL(BUILD_DLL, test $BUILD_IT = true, true, false)
AC_SUBST(BUILD_IT)

AC_CONFIG_FILES([
	libwfut-0.1.pc
	Makefile
	libwfut/Makefile
	tools/Makefile
	man/Makefile
	man/man1/Makefile
])
AC_OUTPUT
