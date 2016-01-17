# i18n handling for The Butterfly Effect
# Note that the actual translations are handled through transifex

find_package(Qt5LinguistTools REQUIRED)
add_definitions(${QT_DEFINITIONS})

# note that tbe_en.ts is missing here, english is the default.
set(TSfiles
	tbe_de.ts
	tbe_nl.ts
)
qt5_create_translation(QMfiles ${TSfiles})

add_custom_target (translations DEPENDS ${QMfiles})
add_dependencies(tbe translations)

install(FILES ${QMfiles}
	DESTINATION ${TBE_I18N_DIR}
)
