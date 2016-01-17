# i18n handling for The Butterfly Effect
# Note that the actual translations are handled through transifex

SET (ALL_LINGUAS nl de)

##############################################################################
###  TBE Application translations
###
find_package(Qt5LinguistTools REQUIRED)
add_definitions(${QT_DEFINITIONS})

# note that tbe_en.ts is missing here, english is the default.
SET (ALLTSFILES "")
FOREACH(lingua ${ALL_LINGUAS})
	SET  (_OUTPUT_FILE tbe_${lingua}.ts)
	LIST (APPEND ALLTSFILES ${_OUTPUT_FILE})
ENDFOREACH(lingua)
SET (TSfiles ${ALLTSFILES})
qt5_create_translation(QMfiles ${TSfiles})

add_custom_target (translations DEPENDS ${QMfiles})
add_dependencies(tbe translations)

install(FILES ${QMfiles}
	DESTINATION ${TBE_I18N_DIR}
)


##############################################################################
# Level translations
#
# Not using FindGettext because it appears to be broken?
# The below is a simplified version of gnucash/po/CMakeLists.txt
#
FIND_PROGRAM(MSGFMT msgfmt)
SET (CATALOGS "")
FOREACH(lingua ${ALL_LINGUAS})
	SET(_OUTPUT_FILE ${lingua}.gmo)
	LIST(APPEND CATALOGS ${_OUTPUT_FILE})
	ADD_CUSTOM_COMMAND(
		OUTPUT ${_OUTPUT_FILE}
		COMMAND ${MSGFMT} -o ${_OUTPUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/${lingua}.po
	)
  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${lingua}.gmo RENAME tbe_levels.mo DESTINATION ${TBE_I18N_DIR}/${lingua}/LC_MESSAGES)
ENDFOREACH(lingua)
ADD_CUSTOM_TARGET(po-gmo ALL DEPENDS ${CATALOGS})
