SET(main_SOURCES
    backtrace.cpp
    main.cpp
)
SET (main_HEADERS
    tbe_global.h.in
    tbe_paths.h.in
)

SUBDIRS(Box2D)
INCLUDE(control/control.txt)
INCLUDE(loadsave/loadsave.txt)
INCLUDE(model/model.txt)
INCLUDE(view/view.txt)

include_directories(
    Box2D
    control
    loadsave
    model
    view
    ${CMAKE_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${CMAKE_WORK_DIR}
    ${QT_INCLUDES}
    .
    /opt/local/include
)

if (MINGW)
    add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tberc.o
                     COMMAND windres.exe -I${CMAKE_SOURCE_DIR}
                     -i${CMAKE_SOURCE_DIR}/installer/NSIS/tberc.rc
                     -o${CMAKE_CURRENT_BINARY_DIR}/tberc.o )
    set(rc_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/tberc.o)
else ()
    set(rc_SOURCES)
endif ()

SET(tbe_SOURCES ${control_SOURCES} ${loadsave_SOURCES} ${main_SOURCES} ${model_SOURCES} ${view_SOURCES}
                ${rc_SOURCES})
SET(tbe_HEADERS ${control_HEADERS} ${loadsave_HEADERS} ${main_HEADERS} ${model_HEADERS} ${view_HEADERS})
SET(tbe_UIFORMS ${control_UIFORMS} ${loadsave_UIFORMS} ${main_UIFORMS} ${model_UIFORMS} ${view_UIFORMS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpointer-arith -DQT_NO_ASCII_CAST -std=gnu++0x")

# all Qt-specifics
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Xml)
find_package(Qt5Svg)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_package(Qt5Test)
endif()
ADD_DEFINITIONS(${QT_DEFINITIONS})

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# do no longer automatically moc all header files:
#QT4_WRAP_CPP(tbe_HEADERS ${control_HEADERS} ${view_HEADERS} ${model_HEADERS} ${loadsave_HEADERS})

QT5_WRAP_UI(tbe_FORMS_HEADERS ${tbe_UIFORMS})
QT5_ADD_RESOURCES(tbe_RESOURCES_RCC ${view_RESOURCES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTSVG TRUE)

ADD_EXECUTABLE(tbe 
    ${tbe_SOURCES}
    ${tbe_HEADERS}
    ${tbe_FORMS_HEADERS}
    ${tbe_RESOURCES_RCC}
)

# If you want to run with the sanitizers, enable the cmake plugin.
# It is suggested that you run 'make clean sanitizers' from the root of the
# project, this will also enable compilation/linking with clang.
if (ECM_ENABLE_SANITIZERS)
    find_package(ECM REQUIRED NO_MODULE)
    set(CMAKE_MODULE_PATH ${ECM_MODULE_DIR})
    include(ECMEnableSanitizers)
endif (ECM_ENABLE_SANITIZERS)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    TARGET_LINK_LIBRARIES(tbe Qt5::Xml Qt5::Svg Qt5::Test Box2D)
else()
    TARGET_LINK_LIBRARIES(tbe Qt5::Xml Qt5::Svg Box2D)
endif ()

IF (WIN32)
    TARGET_LINK_LIBRARIES(tbe ${CMAKE_SOURCE_DIR}/src/libintl/libintl.dll.a)
ENDIF ()

if (APPLE)
    # Detect if the "port" command is valid on this system; if so, return full path
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
endif ()

IF (APPLE)
    TARGET_LINK_LIBRARIES(tbe ${LIBINTL_LIBRARIES})
ENDIF ()
 
INSTALL(TARGETS tbe DESTINATION ${TBE_BIN_DIR})
