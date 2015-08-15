#i18n handling for The Butterfly Effect

find_package(Qt4 REQUIRED)
add_definitions(${QT_DEFINITIONS})

set(TSfiles
	tbe_be.ts
	tbe_en.ts
	tbe_es.ts
	tbe_lt.ts
	tbe_ms.ts
	tbe_nl.ts
	tbe_ru.ts
	tbe_sv.ts
	tbe_uk.ts
)
qt4_create_translation(QMfiles ${TSfiles})

add_custom_target (translations DEPENDS ${QMfiles})
add_dependencies(tbe translations)

install(FILES ${QMfiles}
	DESTINATION i18n
)
