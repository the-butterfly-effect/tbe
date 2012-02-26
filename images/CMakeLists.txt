# this CMakeLists.txt file handles the images directory:
#  * make sure all files are put into the 'images' directory itself
#    (because TBE is lazy and only looks in the ../images/ directory)
#  * concatenate all README files to a single file that contains info 
#    on all files in the images directory

cmake_minimum_required(VERSION 2.8.6)

set(READMEtotal "This file lists the orgins of all files in this directory.\n" 
  "It is generated during build from all READMEs in various directories.\n"
  "Note that several files mentioned could be missing here if you are\n"
  "looking at the installed version - please look at the SVN repo instead."
  "\n\n\n")
file(GLOB_RECURSE READMEfiles "README")
foreach(f ${READMEfiles})
   file(READ ${f} READMEf)
   set(READMEtotal ${READMEtotal} "******* " ${f} " ******\n" ${READMEf} "\n")
endforeach(f)
file(WRITE README.image.origins.txt ${READMEtotal})
install(FILES README.image.origins.txt
	DESTINATION images
)

include("src/CMakeLists.txt")
#include("illustrations/illustrations.txt")
include("icons/CMakeLists.txt")
include("textures/CMakeLists.txt")

