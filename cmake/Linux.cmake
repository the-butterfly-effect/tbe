# i.e. Linux variants
set (CMAKE_INSTALL_PREFIX "/usr")
set(TBE_BIN_DIR     games)
set(TBE_LEVELS_DIR  share/games/tbe/levels)
set(TBE_IMAGES_DIR  share/games/tbe/images)
set(TBE_I18N_DIR    share/games/tbe/i18n)
set(TBE_DOC_DIR     share/doc/tbe)
set(TBE_DESKTOP_DIR share/applications)
set(TBE_ICON_DIR    share/icons)
# set(TBE_TARGET_PREFIX "../")



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

set(CPACK_RPM_PACKAGE_REQUIRES "libQt5Core5 >= 5.2.0")
#set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/installer/debian/postinst;${CMAKE_CURRENT_SOURCE_DIR}/installer/debian/prerm;" )
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5-svg (>=5.2), libqtgui5 (>=5.2), libqt5-xml (>=5.2), libqtcore5 (>=5.2)")
set(CPACK_DEBIAN_PACKAGE_SECTION "games")

set(CPACK_SET_DESTDIR "ON")
