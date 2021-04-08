cmake_minimum_required (VERSION 2.8.12)
project (thebutterflyeffect)

option(WITH_DOCS "build package with(out) documentation")

# add the custom cmake scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (NOT (WIN32 OR APPLE))
    set(LINUX 1)
endif ()

# Win and Mac both have a rather flat directory layout.
# At least on Win, the CMAKE_INSTALL_PREFIX is the location inside
# 'build' where all files are copied prior to creating the package.
if (WIN32)
    set(TBE_LEVELS_DIR       "levels")
    set(TBE_IMAGES_DIR       "images")
    set(TBE_I18N_DIR         "i18n")
    set(TBE_DOC_DIR          "doc")
    set(TBE_DESKTOP_DIR      "applications")
    set(TBE_ICON_DIR         "icons")
    set(TBE_TARGET_PREFIX    "./")
    set(TBE_BIN_DIR          ".")
    set(CMAKE_INSTALL_PREFIX ".")
elseif (APPLE)
    set(TBE_TOPLEVEL_DIR tbe.app/Contents)
    set(TBE_RESOURCES_DIR ${TBE_TOPLEVEL_DIR}/Resources)
    set(TBE_LEVELS_DIR    ${TBE_RESOURCES_DIR}/levels)
    set(TBE_IMAGES_DIR    ${TBE_RESOURCES_DIR}/images)
    set(TBE_I18N_DIR      ${TBE_RESOURCES_DIR}/i18n)
    set(TBE_DOC_DIR       ${TBE_RESOURCES_DIR}/doc)
    set(TBE_DESKTOP_DIR   ${TBE_RESOURCES_DIR}/applications)
    set(TBE_ICON_DIR      ${TBE_RESOURCES_DIR}/icons)
    set(TBE_TARGET_PREFIX "../../../")
    set(TBE_BIN_DIR       ${TBE_TOPLEVEL_DIR}/MacOS)
    set(CMAKE_INSTALL_PREFIX installprefix)
elseif (LINUX)
    set(CMAKE_INSTALL_PREFIX "/usr")
    set(TBE_BIN_DIR       games)
    set(TBE_LEVELS_DIR    share/games/tbe/levels)
    set(TBE_IMAGES_DIR    share/games/tbe/images)
    set(TBE_I18N_DIR      share/games/tbe/i18n)
    set(TBE_DOC_DIR       share/doc/tbe)
    set(TBE_DESKTOP_DIR   share/applications)
    set(TBE_ICON_DIR      share/icons)
    set(TBE_TARGET_PREFIX "../")
endif()    

configure_file(
    ${PROJECT_SOURCE_DIR}/src/tbe_paths.h.in
    ${PROJECT_BINARY_DIR}/tbe_paths.h
)

add_subdirectory(imagery)
add_subdirectory(src)
add_subdirectory(i18n)

# use .. because TBE_LEVELS_DIR already ends in 'levels'
install(DIRECTORY levels
    DESTINATION ${TBE_LEVELS_DIR}/..
    PATTERN "test" EXCLUDE
)

if (WITH_DOCS)
    install(
    FILES
        COPYING
        AUTHORS
        README.md
    DESTINATION ${TBE_DOC_DIR}
    )
endif ()

set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/DESCRIPTION")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Butterfly Effect is a computer physics puzzle game.")
set(CPACK_PACKAGE_NAME "the-butterfly-effect.org")
set(CPACK_PACKAGE_VENDOR "the-butterfly-effect.org")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "9.4")
set(CPACK_PACKAGE_VERSION_PATCH "git")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# use rpavlik's cmake scripts to fetch the current git version hash
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_VERSION_HASH)
configure_file(
    ${PROJECT_SOURCE_DIR}/src/tbe_version.h.in
    ${PROJECT_BINARY_DIR}/tbe_version.h
)
configure_file(
    ${PROJECT_SOURCE_DIR}/src/tbe_global.h.in
    ${PROJECT_BINARY_DIR}/tbe_global.h
)

set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_IGNORE_FILES /.git/.gitignore/;*~;/build/;usr)
set(CPACK_SOURCE_STRIP_FILES "")
set(CPACK_STRIP_FILES ON)

###
### Packaging for Win32, Apple and Linux
###
if (WIN32)
    # There is a bug in NSIS that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backslashes.
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
    set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/imagery/illustrations\\\\installer-top-icon.bmp")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}\\\\COPYING")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}\\\\README.md")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "tbe.exe")
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY "${CMAKE_INSTALL_PREFIX}\\\\")
    set(CPACK_NSIS_DISPLAY_NAME "The Butterfly Effect")
    set(CPACK_NSIS_HELP_LINK "http://www.the-butterfly-effect.org")
    set(CPACK_NSIS_URL_INFO_ABOUT "http://www.the-butterfly-effect.org")
    set(CPACK_NSIS_CONTACT "info@the-butterfly-effect.org")
    set(CPACK_NSIS_MODIFY_PATH OFF)
    #set(CPACK_NSIS_MUI_FINISHPAGE_RUN "tbe.exe")

    # Supply the DLL files on Windows - no static linking :-(
    # Technically speaking, this is a hack. But who cares?
    set(QTMYLIB "C:/Qt/5.5/mingw492_32/bin/")
    install(
    FILES
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
    file(GLOB qtqmfiles ${QTMYLIB}/../translations/qt_*.qm)
    install(FILES ${qtqmfiles} DESTINATION ${TBE_I18N_DIR})
    # Windows also needs qpa file in the bindir/platforms directory
    install(FILES "${QTMYLIB}/../plugins/platforms/qwindows.dll" DESTINATION "${TBE_BIN_DIR}/platforms/")
elseif (APPLE)
    set_target_properties(tbe PROPERTIES MACOSX_BUNDLE false)
    set(CPACK_GENERATOR tgz)
    set_source_files_properties(${CMAKE_INSTALL_PREFIX} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    #set(CPACK_SET_DESTDIR "ON")
    install(FILES ${CMAKE_SOURCE_DIR}/installer/macosx/Info.plist DESTINATION ${TBE_TOPLEVEL_DIR})
    install(FILES ${CMAKE_SOURCE_DIR}/installer/macosx/tbe-icon.icns DESTINATION ${TBE_RESOURCES_DIR})
elseif (LINUX)
    set(CPACK_PACKAGE_CONTACT "info@the-butterfly-effect.org")
    set(CPACK_STRIP_FILES "tbe")
    install(DIRECTORY installer/icons/hicolor DESTINATION ${TBE_ICON_DIR} )
    install(FILES installer/tbe.desktop DESTINATION ${TBE_DESKTOP_DIR} )

    if (RPM)
        # DEB and RPM packages
        set(CPACK_GENERATOR "DEB" "RPM")
        set(CPACK_PACKAGING_INSTALL_PREFIX "/")
    else ()
        # TGZ and SH packagers
        # for UNIX (including Linux), no path included
        set(CPACK_GENERATOR "STGZ" "TGZ")
        set(CPACK_PACKAGING_INSTALL_PREFIX "/")
        # we use a special License file for the self-extracting archive
        # so we can display both license info *and* give a few instructions
        SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/installer/License")
    endif ()

    set(CPACK_RPM_PACKAGE_REQUIRES "libQt5Core5 >= 5.2.0")
    #set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/installer/debian/postinst;${CMAKE_CURRENT_SOURCE_DIR}/installer/debian/prerm;" )
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5-svg (>=5.2), libqtgui5 (>=5.2), libqt5-xml (>=5.2), libqtcore5 (>=5.2)")
    set(CPACK_DEBIAN_PACKAGE_SECTION "games")

    set(CPACK_SET_DESTDIR "ON")
endif ()

set(CPACK_PACKAGE_EXECUTABLES "tbe" "tbe.exe")
include(CPack)
