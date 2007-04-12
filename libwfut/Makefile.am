lib_LTLIBRARIES = libwfut-0.1.la

libwfut_0_1_la_LDFLAGS = -version-info @LIBWFUT_VERSION_INFO@
#libwfut_0_1_la_LIBADD = ../tinyxml/libtinyxml.a

INCLUDES=-I${top_srcdir}

if BUILD_DLL
libwfut_0_1_la_CPPFLAGS=-DBUILD_LIBWFUT_DLL
endif

libwfut_0_1_la_SOURCES = \
	ChannelParser.cpp \
	ChannelWriter.cpp \
	crc32.cpp \
	Encoder.cpp \
	IO.cpp \
	FileParser.cpp \
	FileWriter.cpp \
	MirrorParser.cpp \
	WFUT.cpp \
	platform.cpp\
	tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp \
	tinystr.h tinyxml.h

libwfutdir = $(includedir)/libwfut-0.1/libwfut/

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
