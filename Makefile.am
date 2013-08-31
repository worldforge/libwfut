SUBDIRS=libwfut tools man python

ACLOCAL_AMFLAGS=-I m4

EXTRA_DIST=autogen.sh libwfut.spec libwfut.spec.in libwfut.dox support/ChangeLog-CVS support/generate-ChangeLog.sh

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libwfut-0.2.pc

dist-hook:
	sh $(top_srcdir)/support/generate-ChangeLog.sh $(top_srcdir) $(distdir) 7b39d0022c60968728d1178691f2430150791ed3

