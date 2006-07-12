lib_LTLIBRARIES = libwfut-0.1.la

libwfut_0_1_la_LDFLAGS = -version-info @LIBWFUT_VERSION_INFO@
libwfut_0_1_la_LIBADD = ../tinyxml/libtinyxml.a

INCLUDES=-I${top_srcdir}

if BUILD_DLL
libwfut_0_1_la_CPPFLAGS=-DBUILD_LIBWFUT_DLL
endif

libwfut_0_1_la_SOURCES = \
	ChannelParser.cpp \
	ChannelWriter.cpp \
	IO.cpp \
	FileParser.cpp \
	FileWriter.cpp \
	WFUT.cpp

libwfutdir = $(includedir)/libwfut-0.1/libwfut/

libwfut_HEADERS = \
	header.h \
	ChannelIO.h \
	FileIO.h \
	IO.h \
	WFUT.h \
	ChannelFileList.h
