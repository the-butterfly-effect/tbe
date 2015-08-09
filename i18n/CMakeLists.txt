#i18n handling for The Butterfly Effect

find_package(Qt4 REQUIRED)
add_definitions(${QT_DEFINITIONS})

set(TSfiles
	tbe_nl.ts
	tbe_es.ts
	tbe_ru.ts
	tbe_ms.ts
	tbe_uk.ts
)
qt4_create_translation(QMfiles ${TSfiles})

add_custom_target (translations DEPENDS ${QMfiles})
add_dependencies(tbe translations)

install(FILES ${QMfiles}
	DESTINATION i18n
)
