SET(main_SOURCES
    backtrace.cpp
    main.cpp
    tbe_global.h
    tbe_paths.h
)
SET (main_HEADERS
    tbe_global.h
    tbe_paths.h
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
    ${CMAKE_WORK_DIR}
    ${QT_INCLUDES}
    .
)

if (MINGW)
 add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tberc.o
                     COMMAND windres.exe -I${CMAKE_SOURCE_DIR}
                     -i${CMAKE_SOURCE_DIR}/installer/NSIS/tberc.rc
                     -o${CMAKE_CURRENT_BINARY_DIR}/tberc.o )
 set(rc_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/tberc.o)
else (MINGW)
 set(rc_SOURCES)
endif (MINGW)

SET(tbe_SOURCES ${control_SOURCES} ${loadsave_SOURCES} ${main_SOURCES} ${model_SOURCES} ${view_SOURCES}
                ${rc_SOURCES})
SET(tbe_HEADERS ${control_HEADERS} ${loadsave_HEADERS} ${main_HEADERS} ${model_HEADERS} ${view_HEADERS})
SET(tbe_UIFORMS ${control_UIFORMS} ${loadsave_UIFORMS} ${main_UIFORMS} ${model_UIFORMS} ${view_UIFORMS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpointer-arith -DQT_NO_ASCII_CAST -std=gnu++0x")

# all Qt-specifics
find_package(Qt4 REQUIRED)
ADD_DEFINITIONS(${QT_DEFINITIONS})

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# do no longer automatically moc all header files:
#QT4_WRAP_CPP(tbe_HEADERS ${control_HEADERS} ${view_HEADERS} ${model_HEADERS} ${loadsave_HEADERS})

QT4_WRAP_UI(tbe_FORMS_HEADERS ${tbe_UIFORMS})
QT4_ADD_RESOURCES(tbe_RESOURCES_RCC ${view_RESOURCES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTSVG TRUE)

ADD_EXECUTABLE(tbe 
    ${tbe_SOURCES}
    ${tbe_HEADERS}
    ${tbe_FORMS_HEADERS}
    ${tbe_RESOURCES_RCC}
)

INCLUDE(${QT_USE_FILE})
TARGET_LINK_LIBRARIES(tbe ${QT_LIBRARIES} Box2D)

# Supply the DLL files on Windows - no static linking :-(
# Technically speaking, this is a hack. But who cares?
if(MINGW)
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
endif(MINGW)

INSTALL(TARGETS tbe DESTINATION ${TBE_BIN_DIR})
