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
