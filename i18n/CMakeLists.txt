# i18n handling for The Butterfly Effect
# Note that the actual translations are handled through transifex

find_package(Qt5LinguistTools REQUIRED)
add_definitions(${QT_DEFINITIONS})

# note that tbe_en.ts is missing here, english is the default.
set(TSfiles
	tbe_be.ts
	tbe_de.ts
	tbe_es.ts
	tbe_fr.ts
	tbe_gl.ts
	tbe_is.ts
	tbe_lt.ts
	tbe_ms.ts
	tbe_nl.ts
	tbe_ru.ts
	tbe_sv.ts
	tbe_uk.ts
)
qt5_create_translation(QMfiles ${TSfiles})

add_custom_target (translations DEPENDS ${QMfiles})
add_dependencies(tbe translations)

install(FILES ${QMfiles}
	DESTINATION ${TBE_I18N_DIR}
)
