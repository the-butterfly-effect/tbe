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
INCLUDE(qml/qml.txt)
INCLUDE(view/view.txt)

include_directories(
    Box2D
    control
    loadsave
    model
    view
    qml
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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpointer-arith -DQT_NO_ASCII_CAST")

# all Qt-specifics
find_package(Qt5 COMPONENTS Core Quick QuickWidgets Svg Test Widgets Xml)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("***** Building DEBUG version *****")
    set(USE_QT5TEST Test)
endif()
ADD_DEFINITIONS(${QT_DEFINITIONS})

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

QT5_WRAP_UI(tbe_FORMS_HEADERS ${tbe_UIFORMS})
QT5_ADD_RESOURCES(tbe_RESOURCES_RCC ${view_RESOURCES} ${qml_RESOURCES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTSVG TRUE)

ADD_EXECUTABLE(tbe 
    ${tbe_SOURCES}
    ${tbe_HEADERS}
    ${tbe_FORMS_HEADERS}
    ${tbe_RESOURCES_RCC}
)
QT5_USE_MODULES(tbe Core Widgets Quick QuickWidgets Xml Svg ${USE_QT5TEST})

# If you want to run with the sanitizers, enable the cmake plugin.
# It is suggested that you run 'make clean sanitizers' from the root of the
# project, this will also enable compilation/linking with clang.
if (ECM_ENABLE_SANITIZERS)
    find_package(ECM REQUIRED NO_MODULE)
    set(CMAKE_MODULE_PATH ${ECM_MODULE_DIR})
    include(ECMEnableSanitizers)
endif (ECM_ENABLE_SANITIZERS)

# make sure to link against our static Box2D library
TARGET_LINK_LIBRARIES(tbe Box2D)

IF (APPLE)
    include(MacOSX-libs)
ENDIF()

IF (WIN32)
    TARGET_LINK_LIBRARIES(tbe ${CMAKE_SOURCE_DIR}/src/libintl/libintl.dll.a)
ENDIF ()

 
INSTALL(TARGETS tbe DESTINATION ${TBE_BIN_DIR})
