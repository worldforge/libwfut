cmake_minimum_required(VERSION 3.2)

project(libwfut)
set(CMAKE_CXX_STANDARD 14)
include(GNUInstallDirs)
include(FindPkgConfig)

# Version setup

set(LIBWFUT_VERSION_MAJOR 0)
set(LIBWFUT_VERSION_MINOR 2)
set(LIBWFUT_VERSION_PATCH 3)

set(VERSION ${LIBWFUT_VERSION_MAJOR}.${LIBWFUT_VERSION_MINOR}.${LIBWFUT_VERSION_PATCH})
set(SUFFIX -${LIBWFUT_VERSION_MAJOR}.${LIBWFUT_VERSION_MINOR})

set(LIBWFUT_ABI_CURRENT 2)
set(LIBWFUT_ABI_REVISION 1)
set(LIBWFUT_ABI_AGE 1)
math(EXPR LIBWFUT_SOVERSION ${LIBWFUT_ABI_CURRENT}-${LIBWFUT_ABI_AGE})
set(LIBWFUT_ABI_VERSION ${LIBWFUT_SOVERSION}.${LIBWFUT_ABI_AGE}.${LIBWFUT_ABI_REVISION})

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories("${PROJECT_SOURCE_DIR}")
# Tell Tiny XML to use STL classes.
add_definitions(-DTIXML_USE_STL=1)
add_definitions(-DVERSION="${VERSION}")

# Meta data

set(LIBNAME "wfut${SUFFIX}")
set(DESCRIPTION "A client side C++ implementation of WFUT (WorldForge Update Tool). It is designed to be used by client applications in downloading data file updates from a web server.")


# This macro defines a library
macro(wf_add_library _LIB_NAME _SOURCE_FILES_VAR _HEADER_FILES_VAR)

    add_library(${_LIB_NAME} SHARED ${${_SOURCE_FILES_VAR}})
    set_target_properties(${_LIB_NAME} PROPERTIES
            VERSION ${LIBWFUT_ABI_VERSION}
            SOVERSION ${LIBWFUT_SOVERSION}
            )

    install(TARGETS ${_LIB_NAME}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

    foreach (file ${${_HEADER_FILES_VAR}})
        get_filename_component(dir ${file} DIRECTORY)
        install(FILES ${file} DESTINATION include/${PROJECT_NAME}${SUFFIX}/${dir})
    endforeach ()

    set(PKG_CONFIG_LIBS "-l${_LIB_NAME} ${PKG_CONFIG_LIBS}")

    target_link_libraries(${_LIB_NAME} ${WF_LIBRARIES})

endmacro()

#Add a "check" make target for tests to sync with other Worldforge components.
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

pkg_check_modules(WF REQUIRED sigc++-2.0>=2.0.0 libcurl>=7.16.0)

link_directories(${WF_LIBRARY_DIRS})
include_directories(${WF_INCLUDE_DIRS})
# Populate for pkg-config
set(REQUIRES "sigc++-2.0 libcurl")

# Define source files for libraries

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
        libwfut/tinystr.cpp
        libwfut/tinyxml.cpp
        libwfut/tinyxmlerror.cpp
        libwfut/tinyxmlparser.cpp
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
        libwfut/tinystr.h
        libwfut/tinyxml.h
        libwfut/types.h
        libwfut/WFUT.h)

wf_add_library(${LIBNAME} SOURCE_FILES HEADER_FILES)

# The "wfut" tool
add_executable(wfut tools/wfut.cpp)
target_link_libraries(wfut ${LIBNAME})
install(TARGETS wfut
        DESTINATION ${CMAKE_INSTALL_BINDIR})

# pkg-config files
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}${SUFFIX}.pc @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}${SUFFIX}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

# RPM spec files
#TODO: fix these
configure_file(${PROJECT_NAME}.spec.in ${PROJECT_NAME}.spec @ONLY)

# man files
install(FILES man/man1/wfut.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

# Doxygen support, exports a "docs" target.

configure_file(libwfut.dox.in libwfut.dox @ONLY)
find_package(Doxygen)

if (DOXYGEN_FOUND)

    set(DOXYGEN_INPUT ${CMAKE_CURRENT_BINARY_DIR}/libwfut.dox)
    set(DOXYGEN_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/doc)

    add_custom_command(
            OUTPUT ${DOXYGEN_OUTPUT}
            COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
            COMMAND ${CMAKE_COMMAND} -E echo "Done."
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${DOXYGEN_INPUT}
    )

    add_custom_target(docs DEPENDS ${DOXYGEN_OUTPUT})

endif (DOXYGEN_FOUND)

add_custom_command(
        OUTPUT ChangeLog
        COMMAND ${CMAKE_SOURCE_DIR}/support/generate-ChangeLog.sh ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR} 8bd480b053190ffde2afe33af66f484953036f5a
)
add_custom_target(changelog DEPENDS ChangeLog)


# Packaging (for source tarballs

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${DESCRIPTION})
set(CPACK_PACKAGE_VENDOR "Worldforge")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${LIBWFUT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${LIBWFUT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${LIBWFUT_VERSION_PATCH}")
#set(CPACK_INSTALL_SCRIPT "sh ${CMAKE_SOURCE_DIR}/support/generate-ChangeLog.sh ${CMAKE_SOURCE_DIR} ${CPACK_PACKAGE_INSTALL_DIRECTORY} 8bd480b053190ffde2afe33af66f484953036f5a")

set(CPACK_SOURCE_GENERATOR TBZ2 ZIP)

set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${VERSION}" CACHE INTERNAL "tarball basename")

set(CPACK_SOURCE_IGNORE_FILES
        # no hidden files
        "/\\\\..+$"
        "~$"
        )

include(CPack)