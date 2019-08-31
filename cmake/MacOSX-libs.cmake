set_target_properties( tbe PROPERTIES MACOSX_BUNDLE false )

find_package( Intl REQUIRED )

target_include_directories(tbe PUBLIC ${Intl_INCLUDE_DIRS})
target_link_libraries(tbe ${Intl_LIBRARIES})
