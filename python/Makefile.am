SUFFIXES: .cxx .i .lo .la .cpp .o  .obj

.i.cxx:
	${SWIG} -python -c++ -interface py$(@:.cxx=) -I$(top_srcdir) -o $@ $<

INCLUDES = -I${top_srcdir} @PYTHON_INCLUDES@

pythonfvsdir = $(pyexecdir)/libwfut
EXTRA_pythonfvsdir = $(pyexecdir)/libwfut

if HAVE_PYTHON
EXTRA_pythonfvs_PYTHON = \
	__init__.py \
	WFUTCore.py \
	wfut-python.py \
	wfut-server.py \
	WFUTAvahiClient.py

EXTRA_pythonfvs_LTLIBRARIES = pyWFUT.la
else
EXTRA_pythonfvs_PYTHON=
EXTRA_pythonfvs_LTLIBRARIES=
endif

pythonfvs_PYTHON =
pythonfvs_LTLIBRARIES = 

pyWFUT_la_LDFLAGS = -module -avoid-version
pyWFUT_la_LIBADD = ../libwfut/libwfut-0.2.la @LIBWFUT_LIBS@
pyWFUT_la_SOURCES = WFUT.cxx 

CLEANFILES = \
	WFUT.cxx WFUT.py

EXTRA_DIST = __init__.py WFUT.i wfut-python.py WFUTCore.py wfut-server.py WFUTAvahiClient.py
