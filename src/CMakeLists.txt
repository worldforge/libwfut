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
        libwfut/WFUT.cpp)

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
target_sources(${LIBNAME} PRIVATE $<TARGET_OBJECTS:tinyxml>)

target_link_libraries(${LIBNAME} PUBLIC
        libsigcpp::sigc++
        CURL::libcurl
        ZLIB::ZLIB)

if (WIN32)
    target_include_directories(${LIBNAME} PRIVATE "../external/dirent")
endif ()

if (NOT WIN32)
    # The "wfut" tool
    add_executable(wfut-bin wfut.cpp)
    target_compile_options(wfut-bin PRIVATE ${WF_WARNING_FLAGS})
    target_link_libraries(wfut-bin PUBLIC
            ${LIBNAME})
    set_target_properties(wfut-bin
            PROPERTIES OUTPUT_NAME wfut)
    install(TARGETS wfut-bin
            DESTINATION ${CMAKE_INSTALL_BINDIR})
endif ()
