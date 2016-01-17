cmake_minimum_required (VERSION 2.8.5)
project (thebutterflyeffect)

# Simplified version of the cmakelists - just enough to test working with i18n.
#
# Linux variants only.

set (CMAKE_INSTALL_PREFIX "usr")
set(TBE_BIN_DIR     games)
set(TBE_LEVELS_DIR  share/games/tbe/levels)
set(TBE_IMAGES_DIR  share/games/tbe/images)
set(TBE_I18N_DIR    share/games/tbe/i18n)
set(TBE_DOC_DIR     share/doc/tbe)
set(TBE_DESKTOP_DIR share/applications)
set(TBE_ICON_DIR    share/icons)
#set(TBE_TARGET_PREFIX "../")

configure_file(
    ${PROJECT_SOURCE_DIR}/src/tbe_paths.h.in
    ${PROJECT_BINARY_DIR}/tbe_paths.h
    )

# for internal Qt-related debugging sessions
#set(QT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../qt-4.x-src-verify/install")
#set(QT_QMAKE_EXECUTABLE ${QT_ROOT_DIR}/bin/qmake)

add_subdirectory(src)
add_subdirectory(i18n)

# use .. because TBE_LEVELS_DIR already ends in 'levels'
install(DIRECTORY levels
        DESTINATION ${TBE_LEVELS_DIR}/..
        PATTERN "test" EXCLUDE
)

SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/DESCRIPTION")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Butterfly Effect is a computer physics puzzle game.")
SET(CPACK_PACKAGE_NAME "the-butterfly-effect.org")
SET(CPACK_PACKAGE_VENDOR "the-butterfly-effect.org")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "9.3")
SET(CPACK_PACKAGE_VERSION_PATCH "1")
SET(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

configure_file(
    ${PROJECT_SOURCE_DIR}/src/tbe_global.h.in
    ${PROJECT_BINARY_DIR}/tbe_global.h
    )

set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_IGNORE_FILES /.git/.gitignore/;*~;/build/;usr)
set(CPACK_SOURCE_STRIP_FILES "")
set(CPACK_STRIP_FILES ON)

###
### below, you'll find all packaging info specific to Linux.
###
SET(CPACK_PACKAGE_CONTACT "info@the-butterfly-effect.org")
SET(CPACK_STRIP_FILES "tbe")

# TGZ and SH packagers
# for UNIX (including Linux), no path included
SET(CPACK_GENERATOR "TGZ")
set(CPACK_PACKAGING_INSTALL_PREFIX "/")

set(CPACK_RPM_PACKAGE_REQUIRES "libqt5-x11 >= 5.2.0")
#set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/installer/debian/postinst;${CMAKE_CURRENT_SOURCE_DIR}/installer/debian/prerm;" )
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5-svg (>=5.2), libqtgui5 (>=5.2), libqt5-xml (>=5.2), libqtcore5 (>=5.2)")
set(CPACK_DEBIAN_PACKAGE_SECTION "games")

set(CPACK_SET_DESTDIR "ON")

SET(CPACK_PACKAGE_EXECUTABLES "tbe" "tbe.exe")
INCLUDE(CPack)
