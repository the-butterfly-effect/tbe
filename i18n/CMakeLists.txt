#i18n handling for The Butterfly Effect
cmake_minimum_required (VERSION 2.8.6)

find_package(Qt4 REQUIRED)
add_definitions(${QT_DEFINITIONS})

set(TSfiles
	tbe_nl.ts
	tbe_es.ts
)
qt4_create_translation(QMfiles ${TSfiles})

add_custom_target (translations DEPENDS ${QMfiles})

install(FILES ${QMfiles}
	DESTINATION i18n
)
