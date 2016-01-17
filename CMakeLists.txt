cmake_minimum_required (VERSION 2.8.5)
project (thebutterflyeffect)

option(WITH_DOCS "build package with(out) documentation")

if (WIN32 OR APPLE)
    # Win and Mac both have a rather flat directory layout.
    # At least on Win, the CMAKE_INSTALL_PREFIX is the location inside
    # 'build' where all files are copied prior to creating the package.
    set (CMAKE_INSTALL_PREFIX "Resources")
    set(TBE_BIN_DIR     ".")
    set(TBE_LEVELS_DIR  levels)
    set(TBE_IMAGES_DIR  images)
    set(TBE_I18N_DIR    i18n)
    set(TBE_DOC_DIR     doc)
    set(TBE_DESKTOP_DIR applications)
    set(TBE_ICON_DIR    icons)
    set(TBE_TARGET_PREFIX ${CMAKE_INSTALL_PREFIX})
else (WIN32 OR APPLE)
    # i.e. Linux variants
    set (CMAKE_INSTALL_PREFIX "/usr")
    set(TBE_BIN_DIR     games)
    set(TBE_LEVELS_DIR  share/games/tbe/levels)
    set(TBE_IMAGES_DIR  share/games/tbe/images)
    set(TBE_I18N_DIR    share/games/tbe/i18n)
    set(TBE_DOC_DIR     share/doc/tbe)
    set(TBE_DESKTOP_DIR share/applications)
    set(TBE_ICON_DIR    share/icons)
    set(TBE_TARGET_PREFIX "../")
endif (WIN32 OR APPLE)

configure_file(
    ${PROJECT_SOURCE_DIR}/src/tbe_paths.h.in
    ${PROJECT_BINARY_DIR}/tbe_paths.h
)

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

IF(WIN32 OR APPLE)
    ###
    ### below, you'll find all packaging info specific to Win32 or Apple
    ###
    if (WIN32)
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
        set(QTMYLIB "C:/Qt/4.8.6/bin/")
        INSTALL(FILES
            ${QTMYLIB}/libgcc_s_dw2-1.dll
            ${QTMYLIB}/libstdc++-6.dll
            ${QTMYLIB}/libwinpthread-1.dll
            ${QTMYLIB}/QtCore4.dll
            ${QTMYLIB}/QtGui4.dll
            ${QTMYLIB}/QtSvg4.dll
            ${QTMYLIB}/QtXml4.dll
            DESTINATION ${TBE_BIN_DIR}
        )
    endif(WIN32)
    if (APPLE)
        set_target_properties(tbe PROPERTIES MACOSX_BUNDLE true)
        set(CPACK_GENERATOR tgz)
        set_source_files_properties(${CMAKE_INSTALL_PREFIX} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
        # set(CPACK_SET_DESTDIR "ON")
    endif (APPLE)

ELSE(WIN32 OR APPLE)
    ###
    ### below, you'll find all packaging info specific to Linux.
    ###
    SET(CPACK_PACKAGE_CONTACT "info@the-butterfly-effect.org")
    SET(CPACK_STRIP_FILES "tbe")
    install(DIRECTORY installer/icons/hicolor DESTINATION ${TBE_ICON_DIR} )
    install(FILES installer/tbe.desktop DESTINATION ${TBE_DESKTOP_DIR} )

    if("${RPM}")
        # DEB and RPM packages
        SET(CPACK_GENERATOR "DEB" "RPM")
        set(CPACK_PACKAGING_INSTALL_PREFIX "/")
    else("${RPM}")
        # TGZ and SH packagers
        # for UNIX (including Linux), no path included
        SET(CPACK_GENERATOR "STGZ" "TGZ")
        set(CPACK_PACKAGING_INSTALL_PREFIX "/")
        # we use a special License file for the self-extracting archive
        # so we can display both license info *and* give a few instructions
        SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/installer/License")
    endif("${RPM}")

    set(CPACK_RPM_PACKAGE_REQUIRES "libqt5-x11 >= 5.2.0")
    #set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/installer/debian/postinst;${CMAKE_CURRENT_SOURCE_DIR}/installer/debian/prerm;" )
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5-svg (>=5.2), libqtgui5 (>=5.2), libqt5-xml (>=5.2), libqtcore5 (>=5.2)")
    set(CPACK_DEBIAN_PACKAGE_SECTION "games")

    set(CPACK_SET_DESTDIR "ON")

ENDIF(WIN32 OR APPLE)

SET(CPACK_PACKAGE_EXECUTABLES "tbe" "tbe.exe")
INCLUDE(CPack)
