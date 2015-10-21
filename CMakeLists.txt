cmake_minimum_required (VERSION 2.8.5)
project (thebutterflyeffect)

option(WITH_DOCS "build package with(out) documentation")

if (PACK)
   set(CMAKE_INSTALL_PREFIX ".")
else (PACK)
   set(CMAKE_INSTALL_PREFIX /usr)
endif (PACK)

set(TBE_BIN_DIR     ${CMAKE_INSTALL_PREFIX}/games)
set(TBE_LEVELS_DIR  ${CMAKE_INSTALL_PREFIX}/share/games/tbe/levels)
set(TBE_IMAGES_DIR  ${CMAKE_INSTALL_PREFIX}/share/games/tbe/images)
set(TBE_I18N_DIR    ${CMAKE_INSTALL_PREFIX}/share/games/tbe/i18n)
set(TBE_DOC_DIR     ${CMAKE_INSTALL_PREFIX}/share/doc/tbe)
set(TBE_DESKTOP_DIR ${CMAKE_INSTALL_PREFIX}/share/applications)
set(TBE_ICON_DIR    ${CMAKE_INSTALL_PREFIX}/share/icons)

# for internal Qt-related debugging sessions
#set(QT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../qt-4.x-src-verify/install")
#set(QT_QMAKE_EXECUTABLE ${QT_ROOT_DIR}/bin/qmake)

add_subdirectory(imagery)
add_subdirectory(src)
add_subdirectory(i18n)

# use .. because TBE_LEVELS_DIR already ends in 'levels'
install(DIRECTORY levels
	DESTINATION ${TBE_LEVELS_DIR}/..
	PATTERN "test" EXCLUDE
)

if (WITH_DOCS)
  install(FILES COPYING AUTHORS README.md
	  DESTINATION ${TBE_DOC_DIR}
  )
ENDIF(WITH_DOCS)

SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/DESCRIPTION")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Butterfly Effect is a computer physics puzzle game.")
SET(CPACK_PACKAGE_NAME "thebutterflyeffect")
SET(CPACK_PACKAGE_VENDOR "www.the-butterfly-effect.org")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "9.2")
SET(CPACK_PACKAGE_VERSION_PATCH "1")
SET(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_IGNORE_FILES /.git/.gitignore/;*~;/build/;usr)
set(CPACK_SOURCE_STRIP_FILES "")
set(CPACK_STRIP_FILES ON)

IF(WIN32 AND NOT UNIX)
 # There is a bug in NSIS that does not handle full unix paths properly. Make
 # sure there is at least one set of four (4) backslashes.
 SET(CPACK_GENERATOR "NSIS")
 SET(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
 SET(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/imagery/illustrations\\\\installer-top-icon.bmp")
 SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}\\\\COPYING")
 SET(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}\\\\README.md")
 SET(CPACK_NSIS_INSTALLED_ICON_NAME "games\\\\tbe.exe")
 SET(CPACK_NSIS_EXECUTABLES_DIRECTORY "games")
 SET(CPACK_NSIS_DISPLAY_NAME "The Butterfly Effect")
 SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.the-butterfly-effect.org")
 SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.the-butterfly-effect.org")
 SET(CPACK_NSIS_CONTACT "info@the-butterfly-effect.org")
 SET(CPACK_NSIS_MODIFY_PATH OFF)
 #SET(CPACK_NSIS_MUI_FINISHPAGE_RUN "tbe.exe")
ENDIF(WIN32 AND NOT UNIX)

if (UNIX)
 SET(CPACK_PACKAGE_CONTACT "info@the-butterfly-effect.org")
 SET(CPACK_STRIP_FILES "tbe")
 install(DIRECTORY installer/icons/hicolor DESTINATION ${TBE_ICON_DIR} )
 install(FILES installer/tbe.desktop DESTINATION ${TBE_DESKTOP_DIR} )
endif (UNIX)


if (UNIX AND NOT "${RPM}")
 # for UNIX (including Linux), no path included
 SET(CPACK_GENERATOR "STGZ" "TGZ")
 set(CPACK_PACKAGING_INSTALL_PREFIX "/")
 # we use a special License file for the self-extracting archive
 # so we can display both license info *and* give a few instructions
 SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/installer/License")
ENDIF(UNIX AND NOT "${RPM}")

if(UNIX AND "${RPM}")
 # DEB and RPM install in /opt/TheButterflyEffect/*
 SET(CPACK_GENERATOR "DEB" "RPM")
 set(CPACK_PACKAGING_INSTALL_PREFIX "/")
endif(UNIX AND "${RPM}")
set(CPACK_RPM_PACKAGE_REQUIRES "libqt4-x11 >= 4.7.0")
#set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/installer/debian/postinst;${CMAKE_CURRENT_SOURCE_DIR}/installer/debian/prerm;" )
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-svg (>=4.7), libqtgui4 (>=4.7), libqt4-xml (>=4.7), libqtcore4 (>=4.7)")
set(CPACK_DEBIAN_PACKAGE_SECTION "games")

if (UNIX)
  set(CPACK_SET_DESTDIR "ON")
endif (UNIX)

SET(CPACK_PACKAGE_EXECUTABLES "tbe" "tbe.exe")
INCLUDE(CPack)
