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
target_link_libraries(${LIBNAME} tinyxml)

# The "wfut" tool
add_executable(wfut wfut.cpp)
target_compile_options(wfut PRIVATE ${WF_WARNING_FLAGS})
target_link_libraries(wfut ${LIBNAME})
install(TARGETS wfut
        DESTINATION ${CMAKE_INSTALL_BINDIR})
