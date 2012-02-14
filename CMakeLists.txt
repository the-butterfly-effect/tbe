cmake_minimum_required (VERSION 2.8.6)
#project (The Butterfly Effect)

add_subdirectory(src)

install(DIRECTORY levels 
	DESTINATION .
	PATTERN ".svn" EXCLUDE
)

install(DIRECTORY images 
	DESTINATION .
	FILES_MATCHING PATTERN "*.jpg"
	PATTERN "*.png"
	PATTERN "*.svg"
	PATTERN ".svn" EXCLUDE
	PATTERN "src" EXCLUDE
	PATTERN "textures" EXCLUDE
	PATTERN "icons" EXCLUDE
	PATTERN "illustrations" EXCLUDE
	PATTERN "style" EXCLUDE
	)

install(DIRECTORY i18n
	DESTINATION .
	FILES_MATCHING PATTERN "*.qm"
	PATTERN ".svn" EXCLUDE
)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Butterfly Effect")
SET(CPACK_PACKAGE_VENDOR "www.the-butterfly-effect.org")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET(CPACK_PACKAGE_VERSION_MAJOR "milestone")
SET(CPACK_PACKAGE_VERSION_MINOR "MA")
SET(CPACK_PACKAGE_VERSION_PATCH "rc1")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}-${CMake_VERSION_MINOR}")

IF(WIN32 AND NOT UNIX)
 # There is a bug in NSI that does not handle full unix paths properly. Make
 # sure there is at least one set of four (4) backslashes.
 SET(CPACK_GENERATOR "NSIS")
 SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
 SET(CPACK_NSIS_INSTALLED_ICON_NAME "tbe.exe")
 SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} The Butterfly Effect")
 SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.the-butterfly-effect.org")
 SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.the-butterfly-effect.org")
 SET(CPACK_NSIS_CONTACT "info@the-butterfly-effect.org")
 SET(CPACK_NSIS_MODIFY_PATH ON)
ELSE(WIN32 AND NOT UNIX)
 # for UNIX (including Linux)
 SET(CPACK_GENERATOR "STGZ" "DEB" "TGZ" "RPM")
 SET(CPACK_PACKAGE_CONTACT "info@the-butterfly-effect.org")
 SET(CPACK_STRIP_FILES "tbe")
 SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT UNIX)
SET(CPACK_PACKAGE_EXECUTABLES "tbe" "tbe.exe")
INCLUDE(CPack)
