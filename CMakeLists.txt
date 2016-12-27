cmake_minimum_required (VERSION 3.3)
project (the-butterfly-effect)
add_compile_options(-std=c++11)

option(WITH_DOCS "build package with(out) documentation")

# add the custom cmake scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (WIN32)
  include(Win32)
endif()

if (APPLE)
    include(MacOSX)
endif ()

if (UNIX AND NOT APPLE)
    include(Linux)
endif ()

configure_file(
    ${PROJECT_SOURCE_DIR}/src/tbe_paths.h.in
    ${PROJECT_BINARY_DIR}/tbe_paths.h
)

add_subdirectory(imagery)
add_subdirectory(src)
add_subdirectory(i18n)

# use .. because TBE_LEVELS_DIR already ends in 'levels'
install(DIRECTORY levels
    DESTINATION ${TBE_LEVELS_DIR}/..
    PATTERN "test" EXCLUDE
)

if (WITH_DOCS)
    install(FILES COPYING AUTHORS README.md
            DESTINATION ${TBE_DOC_DIR}
    )
ENDIF(WITH_DOCS)

SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/DESCRIPTION")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Butterfly Effect is a computer physics puzzle game.")
SET(CPACK_PACKAGE_NAME "the-butterfly-effect.org")
SET(CPACK_PACKAGE_VENDOR "the-butterfly-effect.org")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "9.4")
SET(CPACK_PACKAGE_VERSION_PATCH "git")
SET(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# use rpavlik's cmake scripts to fetch the current git version hash
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_VERSION_HASH)
configure_file(
    ${PROJECT_SOURCE_DIR}/src/tbe_version.h.in
    ${PROJECT_BINARY_DIR}/tbe_version.h
)

configure_file(
    ${PROJECT_SOURCE_DIR}/src/tbe_global.h.in
    ${PROJECT_BINARY_DIR}/tbe_global.h
)

set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_IGNORE_FILES /.git/.gitignore/;*~;/build/;usr)
set(CPACK_SOURCE_STRIP_FILES "")
set(CPACK_STRIP_FILES ON)
SET(CPACK_PACKAGE_EXECUTABLES "tbe" "tbe.exe")
INCLUDE(CPack)
