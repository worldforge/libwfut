AC_INIT(libwfut, 0.2.2, simon@worldforge.org)

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([libwfut/WFUT.cpp])
AC_CONFIG_HEADERS([config.h])

AC_CANONICAL_HOST
AM_INIT_AUTOMAKE([tar-ustar nostdinc dist-bzip2])

AC_CHECK_TOOL(SWIG, swig)

LIBWFUT_CURRENT=2   ## Increment if interface changes
LIBWFUT_REVISION=1  ## Increment if source changes. 0 if current is incremented
LIBWFUT_AGE=1       ## Increment if current is incremented, but interface is
                    ## backwards compatible

AC_PROG_CXX
AC_PROG_LIBTOOL


AC_ARG_ENABLE(debug,
  [  --enable-debug     enable debug information [default=no]],
  [
    if test "$enableval" = "yes"; then
      CXXFLAGS="$CXXFLAGS -ggdb -O0 -DDEBUG"
    else
      CXXFLAGS="$CXXFLAGS -O3 -DNDEBUG"
    fi
  ],[
    CXXFLAGS="$CXXFLAGS -O3 -DNDEBUG"
  ]
)


## Tell Tiny XML to use STL classes.
CXXFLAGS="$CXXFLAGS -DTIXML_USE_STL=1"

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

PKG_CHECK_MODULES(CURL_PIPE, libcurl >= 7.16.0,
[
	AC_DEFINE(HAVE_CURL_MULTI_PIPELINING, 1, [Define if curl is recent enough to have the pipelining option.])
],
	UNUSED=1
]
)

# Try to find system-wide installation of tinyxml
# If found, add define to config.h, set LIBS and add automake conditional
AC_CHECK_LIB(tinyxml, main,
	[
		AC_DEFINE(HAVE_LIBTINYXML, 1, [Define to 1 if you have the `tinyxml' library (-ltinyxml).])
		LIBS="-ltinyxml $LIBS"
		have_tinyxml=true
	],
	[
		have_tinyxml=false
	]
)
AM_CONDITIONAL(HAVE_LIBTINYXML, [test x$have_tinyxml = xtrue])

AC_LANG_POP(C++)

AC_CHECK_LIB(z, crc32, [], [AC_MSG_ERROR([Unable to find zlib])])

AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)

LIBWFUT_LIB_SUFFIX="-0.2"
LIBWFUT_LIBS="-lwfut$LIBWFUT_LIB_SUFFIX"
LIBWFUT_VERSION_INFO=$LIBWFUT_CURRENT:$LIBWFUT_REVISION:$LIBWFUT_AGE

AC_SUBST(LIBWFUT_LIB_SUFFIX)
AC_SUBST(LIBWFUT_LIBS)
AC_SUBST(LIBWFUT_REQUIRES)
AC_SUBST(LIBWFUT_VERSION_INFO)


# make python binding?
AC_ARG_WITH([python], AS_HELP_STRING([--without-python], [build without Python bindings (default: test)]))

if test "x$with_python" != "xno"; then
  AM_PATH_PYTHON(2.2,,
    [with_python=no
     AC_MSG_WARN([Python not found; disabling Python binding])])
fi

if test "x$with_python" != "xno"; then
  AM_CHECK_PYTHON_HEADERS(,
    [with_python=no
     AC_MSG_WARN([Python headers not found])])
fi

# need SWIG too
if test "x$with_python" != "xno"; then
  AC_CHECK_PROG(HAVE_SWIG, swig, [yes])
  
  if test "x$HAVE_SWIG" != "xyes"; then
    with_python=no
    AC_MSG_WARN([SWIG not found; disabling Python binding])
  else
    with_python=yes
  fi
fi

if test "x$with_python" = "xyes"; then
  AM_CONDITIONAL(HAVE_PYTHON, true)
else
  AM_CONDITIONAL(HAVE_PYTHON, false)
fi



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
	libwfut.spec
	libwfut-0.2.pc
	Makefile
	libwfut/Makefile
	tools/Makefile
	man/Makefile
	man/man1/Makefile
	python/Makefile
])
AC_OUTPUT
