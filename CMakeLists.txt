cmake_minimum_required (VERSION 2.8.6)
#project (The Butterfly Effect)


add_subdirectory(images)
add_subdirectory(src)
add_subdirectory(i18n)


install(DIRECTORY levels 
	DESTINATION .
	PATTERN ".svn" EXCLUDE
)

install(FILES COPYING README AUTHORS
	DESTINATION .
)

SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/DESCRIPTION")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Butterfly Effect is a computer physics puzzle game.")
SET(CPACK_PACKAGE_NAME "TheButterflyEffect")
SET(CPACK_PACKAGE_VENDOR "www.the-butterfly-effect.org")
SET(CPACK_PACKAGE_VERSION_MAJOR "milestone")
SET(CPACK_PACKAGE_VERSION_MINOR "MA")
SET(CPACK_PACKAGE_VERSION_PATCH "rc1")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README")

set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_IGNORE_FILES /.svn/;*~;/build/;usr)
set(CPACK_SOURCE_STRIP_FILES "")

IF(WIN32 AND NOT UNIX)
 # There is a bug in NSI that does not handle full unix paths properly. Make
 # sure there is at least one set of four (4) backslashes.
 SET(CPACK_GENERATOR "NSIS")
 SET(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
 SET(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/images/illustrations\\\\installer-top-icon.bmp")
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

if (UNIX AND NOT RPM)
 # for UNIX (including Linux)
 SET(CPACK_GENERATOR "STGZ" "TGZ")
 SET(CPACK_PACKAGE_CONTACT "info@the-butterfly-effect.org")
 SET(CPACK_STRIP_FILES "tbe")
 set(CPACK_RPM_PACKAGE_REQUIRES "libqt4-x11 >= 4.7.0")
 set(CPACK_PACKAGING_INSTALL_PREFIX "/")
 install(FILES images/illustrations/tbe-icon.png DESTINATION . RENAME TheButterflyEffect.png)
 install(FILES installer/tbe.desktop DESTINATION . RENAME TheButterflyEffect.desktop)
ENDIF(UNIX AND NOT RPM)

if(UNIX AND "$ENV{RPM}")
 SET(CPACK_GENERATOR "DEB" "RPM")
 set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/${CPACK_PACKAGE_NAME}")
 # RPM doesn't like capitalizing in RPM names
 set(CPACK_RPM_PACKAGE_NAME "thebutterflyeffect")
 set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/installer/RPM/post_install.sh)
 set(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/installer/RPM/post_uninstall.sh)
 install(FILES images/illustrations/tbe-icon.png DESTINATION . RENAME TheButterflyEffect.png)
 install(FILES installer/tbe.desktop DESTINATION . RENAME TheButterflyEffect.desktop)
endif(UNIX AND "$ENV{RPM}")

SET(CPACK_PACKAGE_EXECUTABLES "tbe" "tbe.exe")
INCLUDE(CPack)
