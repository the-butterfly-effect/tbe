set(TBE_TOPLEVEL_DIR tbe.app/Contents)
set(TBE_RESOURCES_DIR ${TBE_TOPLEVEL_DIR}/Resources)
set(TBE_LEVELS_DIR  ${TBE_RESOURCES_DIR}/levels)
set(TBE_IMAGES_DIR  ${TBE_RESOURCES_DIR}/images)
set(TBE_I18N_DIR    ${TBE_RESOURCES_DIR}/i18n)
set(TBE_DOC_DIR     ${TBE_RESOURCES_DIR}/doc)
set(TBE_DESKTOP_DIR ${TBE_RESOURCES_DIR}/applications)
set(TBE_ICON_DIR    ${TBE_RESOURCES_DIR}/icons)
set(TBE_TARGET_PREFIX "../../../")
set(TBE_BIN_DIR     ${TBE_TOPLEVEL_DIR}/MacOS)
set(CMAKE_INSTALL_PREFIX installprefix)



set(CPACK_GENERATOR tgz)
set_source_files_properties(${CMAKE_INSTALL_PREFIX} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
# set(CPACK_SET_DESTDIR "ON")
install(FILES
        ${CMAKE_SOURCE_DIR}/installer/macosx/Info.plist
        DESTINATION ${TBE_TOPLEVEL_DIR})
install(FILES
        ${CMAKE_SOURCE_DIR}/installer/macosx/tbe-icon.icns
        DESTINATION ${TBE_RESOURCES_DIR})
