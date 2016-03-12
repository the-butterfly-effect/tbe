# this CMake script will iterate over all directories
# and include the CMakeIncludeMe.txt file in there
#
# it's all hardcoded to make sure that CMake can actually
# do its work - it doesn't like globbing

macro(handledir dirname)
	set(SRCDIRNAME "src/${dirname}")
	include(${SRCDIRNAME}/CMakeIncludeMe.txt)
endmacro(handledir)


handledir("Balloon")
handledir("Balls")
handledir("BowlingBall")
handledir("BowlingPin")
handledir("Butterfly")
handledir("ColaMintBottle")
handledir("Domino")
handledir("Glue")
handledir("NotFound")
handledir("Pingus")
handledir("PolyObjects")
handledir("PostIt")
handledir("Ramp")
#handledir("RatBread")
handledir("RectObjects")
#handledir("StopWatch")
handledir("Scenery")
handledir("SeeSaw")
handledir("TriggerExplosion")
handledir("Wedge")
