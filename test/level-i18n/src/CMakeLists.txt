SET(main_SOURCES
    main.cpp
    test.cpp
    Translator.cpp
)
SET (main_HEADERS
    tbe_global.h.in
    tbe_paths.h.in
    test.h
    Translator.h
)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${CMAKE_WORK_DIR}
    ${QT_INCLUDES}
    .
)

SET(tbe_SOURCES ${main_SOURCES})
SET(tbe_HEADERS ${main_HEADERS})
SET(tbe_UIFORMS ${main_UIFORMS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpointer-arith -DQT_NO_ASCII_CAST -std=c++11")

# all Qt-specifics
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Xml)
find_package(Qt5Svg)
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

#INCLUDE(${QT_USE_FILE})
#TARGET_LINK_LIBRARIES(tbe ${QT_LIBRARIES} Box2D)
TARGET_LINK_LIBRARIES(tbe Qt5::Widgets Qt5::Core Qt5::Xml Qt5::Svg)
IF (WIN32)
    TARGET_LINK_LIBRARIES(tbe ${CMAKE_SOURCE_DIR}/../../src/libintl/libintl.dll.a)
ENDIF (WIN32)

INSTALL(TARGETS tbe DESTINATION ${TBE_BIN_DIR})
