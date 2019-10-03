cmake_minimum_required(VERSION 3.2)

project(libwfut)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
include(GNUInstallDirs)
include(FindPkgConfig)

# Version setup

set(LIBWFUT_VERSION_MAJOR 0)
set(LIBWFUT_VERSION_MINOR 2)
set(LIBWFUT_VERSION_PATCH 4)

set(VERSION ${LIBWFUT_VERSION_MAJOR}.${LIBWFUT_VERSION_MINOR}.${LIBWFUT_VERSION_PATCH})
set(SUFFIX -${LIBWFUT_VERSION_MAJOR}.${LIBWFUT_VERSION_MINOR})

set(LIBWFUT_ABI_CURRENT 2)
set(LIBWFUT_ABI_REVISION 2)
set(LIBWFUT_ABI_AGE 1)
math(EXPR LIBWFUT_SOVERSION ${LIBWFUT_ABI_CURRENT}-${LIBWFUT_ABI_AGE})
set(LIBWFUT_ABI_VERSION ${LIBWFUT_SOVERSION}.${LIBWFUT_ABI_AGE}.${LIBWFUT_ABI_REVISION})

# Set compiler flags
set(WF_WARNING_FLAGS -Wall -Winit-self -Wcast-qual -Wwrite-strings -Wextra -Wundef -Wmissing-declarations -Wno-unused-parameter -Wshadow -Wno-missing-field-initializers -Wno-long-long)

include_directories("${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/external")
# Tell Tiny XML to use STL classes.
add_definitions(-DTIXML_USE_STL=1)
add_definitions(-DVERSION="${VERSION}")

# Meta data

set(LIBNAME "wfut${SUFFIX}")
set(DESCRIPTION "A client side C++ implementation of WFUT (WorldForge Update Tool).")


# This macro defines a library
macro(wf_add_library _LIB_NAME _SOURCE_FILES_VAR _HEADER_FILES_VAR)

    add_library(${_LIB_NAME} SHARED ${${_SOURCE_FILES_VAR}})
    set_target_properties(${_LIB_NAME} PROPERTIES
            VERSION ${LIBWFUT_ABI_VERSION}
            SOVERSION ${LIBWFUT_SOVERSION}
            )
    target_compile_options(${_LIB_NAME} PRIVATE ${WF_WARNING_FLAGS})

    install(TARGETS ${_LIB_NAME}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

    foreach (file ${${_HEADER_FILES_VAR}})
        get_filename_component(dir ${file} DIRECTORY)
        install(FILES ${file} DESTINATION include/${PROJECT_NAME}${SUFFIX}/${dir})
    endforeach ()

    target_link_libraries(${_LIB_NAME} ${WF_LIBRARIES})

endmacro()

# Required for crc32 calculation
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})
link_libraries(${ZLIB_LIBRARIES})

#Add a "check" make target for tests to sync with other Worldforge components.
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

pkg_check_modules(WF REQUIRED sigc++-2.0>=2.0.0 libcurl>=7.16.0)

link_directories(${WF_LIBRARY_DIRS})
include_directories(${WF_INCLUDE_DIRS})
# Populate for pkg-config
set(REQUIRES "sigc++-2.0 libcurl")

# Define source files for libraries

add_subdirectory("external")
add_subdirectory(src)
#add_library(tinyxml
#        libwfut/tinystr.cpp
#        libwfut/tinyxml.cpp
#        libwfut/tinyxmlerror.cpp
#        libwfut/tinyxmlparser.cpp)

# pkg-config files
configure_file(tools/${PROJECT_NAME}.pc.in ${PROJECT_NAME}${SUFFIX}.pc @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}${SUFFIX}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

# RPM spec files
#TODO: fix these
configure_file(tools/${PROJECT_NAME}.spec.in ${PROJECT_NAME}.spec @ONLY)

# man files
install(FILES docs/man/man1/wfut.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

# Doxygen support, exports a "dox" target.

configure_file(docs/Doxyfile.in Doxyfile @ONLY)
find_package(Doxygen)

if (DOXYGEN_FOUND)

    set(DOXYGEN_INPUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    set(DOXYGEN_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/docs)

    add_custom_command(
            OUTPUT ${DOXYGEN_OUTPUT}
            COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
            COMMAND ${CMAKE_COMMAND} -E echo "Done."
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${DOXYGEN_INPUT}
    )

    add_custom_target(dox DEPENDS ${DOXYGEN_OUTPUT})

endif (DOXYGEN_FOUND)

add_custom_command(
        OUTPUT ChangeLog
        COMMAND ${CMAKE_SOURCE_DIR}/support/generate-ChangeLog.sh ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR} 8bd480b053190ffde2afe33af66f484953036f5a
)
add_custom_target(changelog DEPENDS ChangeLog)


# CMake config files
include(CMakePackageConfigHelpers)

configure_package_config_file(tools/Config.cmake.in ${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR)
write_basic_package_version_file(
        ${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${VERSION}
        COMPATIBILITY ExactVersion)
install(FILES
        ${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake
        ${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

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