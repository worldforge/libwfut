set(SOURCE_FILES
        libwfut/ChannelParser.cpp
        libwfut/ChannelWriter.cpp
        libwfut/crc32.cpp
        libwfut/Encoder.cpp
        libwfut/FileParser.cpp
        libwfut/FileWriter.cpp
        libwfut/IO.cpp
        libwfut/MirrorParser.cpp
        libwfut/platform.cpp
        libwfut/WFUT.cpp
        $<TARGET_OBJECTS:tinyxml>)

set(HEADER_FILES
        libwfut/ChannelFileList.h
        libwfut/ChannelIO.h
        libwfut/crc32.h
        libwfut/Encoder.h
        libwfut/FileIO.h
        libwfut/header.h
        libwfut/IO.h
        libwfut/platform.h
        libwfut/types.h
        libwfut/WFUT.h)

wf_add_library(${LIBNAME} SOURCE_FILES HEADER_FILES)

if (WIN32)
	target_include_directories(${LIBNAME} PRIVATE "../external/dirent")
endif()

if (NOT WIN32)
	# The "wfut" tool
	add_executable(wfut wfut.cpp)
	target_compile_options(wfut PRIVATE ${WF_WARNING_FLAGS})
	target_link_libraries(wfut ${LIBNAME})
	if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
		conan_target_link_libraries(wfut)
	endif ()

	install(TARGETS wfut
			DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
