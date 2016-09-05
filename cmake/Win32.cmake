# this cmake file handles custom settings for Win32 builds


# Windows has a rather flat directory layout.
# At least on Win, the CMAKE_INSTALL_PREFIX is the location inside
# 'build' where all files are copied prior to creating the package.
set(TBE_LEVELS_DIR    "levels")
set(TBE_IMAGES_DIR    "images")
set(TBE_I18N_DIR      "i18n")
set(TBE_DOC_DIR       "doc")
set(TBE_DESKTOP_DIR   "applications")
set(TBE_ICON_DIR      "icons")
set(TBE_TARGET_PREFIX "./")
set(TBE_BIN_DIR       ".")
set(CMAKE_INSTALL_PREFIX ".")


# There is a bug in NSIS that does not handle full unix paths properly. Make
# sure there is at least one set of four (4) backslashes.
SET(CPACK_GENERATOR "NSIS")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
SET(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/imagery/illustrations\\\\installer-top-icon.bmp")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}\\\\COPYING")
SET(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}\\\\README.md")
SET(CPACK_NSIS_INSTALLED_ICON_NAME "tbe.exe")
SET(CPACK_NSIS_EXECUTABLES_DIRECTORY "${CMAKE_INSTALL_PREFIX}\\\\")
SET(CPACK_NSIS_DISPLAY_NAME "The Butterfly Effect")
SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.the-butterfly-effect.org")
SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.the-butterfly-effect.org")
SET(CPACK_NSIS_CONTACT "info@the-butterfly-effect.org")
SET(CPACK_NSIS_MODIFY_PATH OFF)
#SET(CPACK_NSIS_MUI_FINISHPAGE_RUN "tbe.exe")

# Supply the DLL files on Windows - no static linking :-(
# Technically speaking, this is a hack. But who cares?
set(QTMYLIB "C:/Qt/5.5/mingw492_32/bin/")
INSTALL(FILES
    ${QTMYLIB}/libgcc_s_dw2-1.dll
    ${QTMYLIB}/libstdc++-6.dll
    ${QTMYLIB}/libwinpthread-1.dll
    ${QTMYLIB}/Qt5Core.dll
    ${QTMYLIB}/Qt5Widgets.dll
    ${QTMYLIB}/Qt5Gui.dll
    ${QTMYLIB}/Qt5Svg.dll
    ${QTMYLIB}/Qt5Xml.dll
    ${CMAKE_SOURCE_DIR}/src/libintl/libiconv2.dll
    ${CMAKE_SOURCE_DIR}/src/libintl/libintl3.dll
    DESTINATION ${TBE_BIN_DIR}
)
# On Windows, package qt's own qm files for standard strings like yes/non/cancel.
FILE(GLOB qtqmfiles ${QTMYLIB}/../translations/qt_*.qm)
INSTALL(FILES ${qtqmfiles}
    DESTINATION ${TBE_I18N_DIR}
)
# Windows also needs qpa file in the bindir/platforms directory
INSTALL(FILES "${QTMYLIB}/../plugins/platforms/qwindows.dll"
    DESTINATION "${TBE_BIN_DIR}/platforms/"
)
