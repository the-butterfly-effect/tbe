cmake_minimum_required (VERSION 2.8.5)
project (thebutterflyeffect)


add_subdirectory(imagery)
add_subdirectory(src)
add_subdirectory(i18n)


install(DIRECTORY levels
	DESTINATION .
	PATTERN ".svn" EXCLUDE
	PATTERN "test" EXCLUDE
)

install(FILES COPYING AUTHORS
	DESTINATION .
)

SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/DESCRIPTION")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Butterfly Effect is a computer physics puzzle game.")
SET(CPACK_PACKAGE_NAME "thebutterflyeffect")
SET(CPACK_PACKAGE_VENDOR "www.the-butterfly-effect.org")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "milestoneA")
SET(CPACK_PACKAGE_VERSION_PATCH "rc2-1")
SET(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_IGNORE_FILES /.svn/;*~;/build/;usr)
set(CPACK_SOURCE_STRIP_FILES "")
set(CPACK_STRIP_FILES ON)

IF(WIN32 AND NOT UNIX)
 # There is a bug in NSI that does not handle full unix paths properly. Make
 # sure there is at least one set of four (4) backslashes.
 SET(CPACK_GENERATOR "NSIS")
 SET(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
 SET(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/imagery/illustrations\\\\installer-top-icon.bmp")
 SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}\\\\COPYING")
 SET(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}\\\\README")
 SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\tbe.exe")
 SET(CPACK_NSIS_DISPLAY_NAME "The Butterfly Effect")
 SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.the-butterfly-effect.org")
 SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.the-butterfly-effect.org")
 SET(CPACK_NSIS_CONTACT "info@the-butterfly-effect.org")
 SET(CPACK_NSIS_MODIFY_PATH OFF)
 SET(CPACK_NSIS_MUI_FINISHPAGE_RUN "tbe.exe")
ENDIF(WIN32 AND NOT UNIX)

if (UNIX)
 SET(CPACK_PACKAGE_CONTACT "info@the-butterfly-effect.org")
 SET(CPACK_STRIP_FILES "tbe")
 install(FILES imagery/illustrations/tbe-icon.png DESTINATION images RENAME TheButterflyEffect-icon.png)
 install(FILES installer/install_icons.sh DESTINATION .
         PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
 install(FILES installer/uninstall_icons.sh DESTINATION .
         PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
 install(FILES installer/sourceforge.net-tbe.desktop DESTINATION .
         PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
endif (UNIX)


if (UNIX AND NOT "$ENV{RPM}")
 # for UNIX (including Linux), no path included
 SET(CPACK_GENERATOR "STGZ" "TGZ")
 set(CPACK_PACKAGING_INSTALL_PREFIX "/")
 # we use a special License file for the self-extracting archive
 # so we can display both license info *and* give a few instructions
 SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/installer/License")
ENDIF(UNIX AND NOT "$ENV{RPM}")

if(UNIX AND "$ENV{RPM}")
 # DEB and RPM install in /opt/TheButterflyEffect/*
 SET(CPACK_GENERATOR "DEB" "RPM")
 set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/${CPACK_PACKAGE_NAME}")

#  FIND_PROGRAM(CMAKE_UNAME uname /bin /usr/bin /usr/local/bin )
#  IF(CMAKE_UNAME)
#    EXEC_PROGRAM(uname ARGS -i OUTPUT_VARIABLE CPACK_RPM_PACKAGE_ARCHITECTURE)
#    IF (${CPACK_RPM_PACKAGE_ARCHITECTURE} MATCHES "x86_64")
#       SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)
#    ELSE(${CPACK_RPM_PACKAGE_ARCHITECTURE} MATCHES "x86_64")
#       SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${CPACK_RPM_PACKAGE_ARCHITECTURE})
#    ENDIF(${CPACK_RPM_PACKAGE_ARCHITECTURE} matches "x86_64")
#  ENDIF(CMAKE_UNAME)
endif(UNIX AND "$ENV{RPM}")
set(CPACK_RPM_PACKAGE_REQUIRES "libqt4-x11 >= 4.7.0")
set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/installer/RPM/post-install.sh)
set(CPACK_RPM_PRE_UNINSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/installer/RPM/pre-uninstall.sh)
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/installer/debian/postinst;${CMAKE_CURRENT_SOURCE_DIR}/installer/debian/prerm;" )
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-svg (>=4.7), libqtgui4 (>=4.7), libqt4-xml (>=4.7), libqtcore4 (>=4.7)")
set(CPACK_DEBIAN_PACKAGE_SECTION "games")

SET(CPACK_PACKAGE_EXECUTABLES "tbe" "tbe.exe")
INCLUDE(CPack)
