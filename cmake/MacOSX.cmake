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



set_target_properties(tbe PROPERTIES MACOSX_BUNDLE false)
set(CPACK_GENERATOR tgz)
set_source_files_properties(${CMAKE_INSTALL_PREFIX} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
# set(CPACK_SET_DESTDIR "ON")
install(FILES
        ${CMAKE_SOURCE_DIR}/installer/macosx/Info.plist
        DESTINATION ${TBE_TOPLEVEL_DIR})
install(FILES
        ${CMAKE_SOURCE_DIR}/installer/macosx/tbe-icon.icns
        DESTINATION ${TBE_RESOURCES_DIR})


##
## Detect if the "port" command is valid on this system; if so, return full path
##
EXECUTE_PROCESS(COMMAND which port RESULT_VARIABLE DETECT_MACPORTS OUTPUT_VARIABLE MACPORTS_PREFIX ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

IF (${DETECT_MACPORTS} EQUAL 0)
    # "/opt/local/bin/port" doesn't have libs, so we get the parent directory
    GET_FILENAME_COMPONENT(MACPORTS_PREFIX ${MACPORTS_PREFIX} DIRECTORY)

    # "/opt/local/bin" doesn't have libs, so we get the parent directory
    GET_FILENAME_COMPONENT(MACPORTS_PREFIX ${MACPORTS_PREFIX} DIRECTORY)

    # "/opt/local" is where MacPorts lives, add `/lib` suffix and link
    LINK_DIRECTORIES(${LINK_DIRECTORIES} ${MACPORTS_PREFIX}/lib)

    MESSAGE("-- Detected MacPorts: ${MACPORTS_PREFIX}")
ENDIF()

find_library(LIBINTL_LIBRARIES 
    NAMES intl
    PATHS /usr/lib /usr/local/lib ${MACPORTS_PREFIX}/lib
)
IF(LIBINTL_LIBRARIES)
    SET(LIBINTL_FOUND TRUE)
ELSE()
    SET(LIBINTL_FOUND FALSE)
ENDIF()
MESSAGE("-- Detected LIBINTL: ${LIBINTL_FOUND} ${LIBINTL_LIBRARIES}")


TARGET_LINK_LIBRARIES(tbe ${LIBINTL_LIBRARIES})
