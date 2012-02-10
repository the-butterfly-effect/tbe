cmake_minimum_required (VERSION 2.8.6)
#project (The Butterfly Effect)

add_subdirectory(src)

install(DIRECTORY levels 
	DESTINATION .
	PATTERN ".svn" EXCLUDE
)

install(DIRECTORY images 
	DESTINATION .
	FILES_MATCHING PATTERN "*.jpg"
	PATTERN "*.png"
	PATTERN "*.svg"
	PATTERN ".svn" EXCLUDE
)
