lib_LTLIBRARIES = libwfut-0.2.la

libwfut_0_2_la_LDFLAGS = -version-info @LIBWFUT_VERSION_INFO@
#libwfut_0_2_la_LIBADD = ../tinyxml/libtinyxml.a

INCLUDES=-I${top_srcdir} -I$(top_builddir)

if BUILD_DLL
libwfut_0_2_la_CPPFLAGS=-DBUILD_LIBWFUT_DLL
endif

libwfut_0_2_la_SOURCES = \
	ChannelParser.cpp \
	ChannelWriter.cpp \
	crc32.cpp \
	Encoder.cpp \
	IO.cpp \
	FileParser.cpp \
	FileWriter.cpp \
	MirrorParser.cpp \
	WFUT.cpp \
	platform.cpp

if !HAVE_LIBTINYXML
libwfut_0_2_la_SOURCES += tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp \
	tinystr.h tinyxml.h
endif

libwfutdir = $(includedir)/libwfut-0.2/libwfut/

libwfut_HEADERS = \
	ChannelFileList.h \
	ChannelIO.h \
	crc32.h \
	Encoder.h \
	header.h \
	FileIO.h \
	types.h \
	IO.h \
	WFUT.h \
	platform.h
