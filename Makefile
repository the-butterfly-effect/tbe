# Makefile to 'simplify' using cmake
#
# Possible targets:
#   - all         - Use only once, it builds everything and installs the built tree for local runs.
#   - slow        - For incremental builds after running 'all'.
#   - clean       - Removes all build artefacts and the installed tree.
#   - sanitizers  - Will build using clang++ and ASAN/USAN/LSAN, the 'google sanitizers'.
#                   The build will output extra messages for run-time problems, useful for debugging.
#   - regression  - Will run a set of levels with its intended solution to validate the game engine
#                   is still operating as expected. Spoilers! For developers only.
#   - package     - Will build installer packages.
#

BUILDDIR=build
# if you want to build Release, just call "make BUILDTYPE=Release all"
BUILDTYPE=Debug
# if you want to build without docs, just call "make WITH_DOCS=off all"
WITH_DOCS=on

all: usr/games/tbe

usr/games/tbe: ${BUILDDIR}/src/tbe

sanitizers:	EXTRACMAKEFLAGS=-DECM_ENABLE_SANITIZERS='undefined' -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_LINKER=clang++ -DCMAKE_CXX_FLAGS= "-fno-omit-frame-pointer -ggdb3"
sanitizers:     ${BUILDDIR}/src/tbe

${BUILDDIR}/src/tbe:
	mkdir -p ${BUILDDIR}
	cd ${BUILDDIR} && cmake ${EXTRACMAKEFLAGS} -DCMAKE_BUILD_TYPE=${BUILDTYPE} -DWITH_DOCS=${WITH_DOCS} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G"CodeBlocks - Unix Makefiles" ..
	cd ${BUILDDIR} && make -j`grep -c ^processor /proc/cpuinfo`
	cd ${BUILDDIR} && make DESTDIR=.. install
	ln -sf usr/games/tbe .

slow:
	cd ${BUILDDIR} && make
	cp ${BUILDDIR}/src/tbe usr/games

regression: usr/games/tbe
	./tbe --regression levels/finished/balloons-do-poof.xml:6, \
	      --regression levels/finished/bouncing_balls.xml:6, \
	      --regression levels/finished/bowling_pin_plays_soccer.xml:17, \
	      --regression levels/finished/brother-plays-penguin-goalie.xml:5, \
	      --regression levels/finished/brother-plays-soccer.xml:8, \
	      --regression levels/finished/butterfly-on-steroids.xml:65, \
	      --regression levels/finished/cola-powered-bike.v2.xml:20, \
              --regression levels/finished/construction_yard.xml:26, \
              --regression levels/finished/contraption1.xml:36, \
              --regression levels/finished/float-balloon-float.xml:21, \
	      --regression levels/needs-polish/geyser.xml:16, \
	      --regression levels/finished/house_of_cards.xml:20, \
	      --regression levels/finished/imperfectbalance.xml:30, \
	      --regression levels/finished/in_the_attic.xml:17, \
	      --regression levels/finished/jumping_around-2.xml:10, \
	      --regression levels/finished/loopings2.xml:22, \
	      --regression levels/finished/save-the-butterfly.xml:28, \
	      --regression levels/needs-polish/spare-the-balloon.xml:25, \
	      --regression levels/finished/the_ball_the_box_and_the_penguin.xml:32, \
	      --regression levels/finished/birds1.xml:13, \
	      --regression levels/needs-polish/002.xml:30, \
	      --regression levels/finished/003.xml:40, \
	      --regression levels/finished/004.xml:15, \
	      --regression levels/finished/006.xml:8, \
	      --regression levels/finished/pingus-1.xml:65, \
	      --regression levels/finished/party-at-office.xml:15, \
	      --regression levels/finished/picnic-0.xml:13, \
	      --regression levels/finished/picnic-1.xml:13, \
	      --regression levels/finished/picnic-3.xml:70, \
              --regression levels/finished/wedge_land.xml:49, \
              --regression levels/finished/005.xml:3, \
              --regression levels/finished/10_kilograms_above_the_ground.xml:17, \
              --regression levels/finished/attack_of_the_killer_cacti.xml:15, \
              --regression levels/finished/bridge-2.xml:10, \
              --regression levels/finished/bridge_gap.xml:10, \
              --regression levels/finished/maze.xml:10, \
              --regression levels/finished/pingu_poppins.xml:40, \
              --regression levels/finished/cola_reaction.xml:60

# levels currently known to fail regression:
failregression: usr/games/tbe
	# zoingandboom (cannot set phone numbers using hints yet)

clean:
	rm -rf ${BUILDDIR} ./tbe usr

distclean:
	rm -rf ${BUILDDIR} usr CPack* CMakeFiles cmake_install.cmake CMakeCache.txt CMakeLists.txt.user ./tbe
	find . -name '*~' -exec rm {} \;

package:
	mkdir -p ${BUILDDIR}
	cd ${BUILDDIR} && cmake -DCMAKE_BUILD_TYPE=Release ..
	cd ${BUILDDIR} && cpack -G TGZ
	cd ${BUILDDIR} && cpack -G STGZ
	cd ${BUILDDIR} && cmake -DRPM=1 -DCMAKE_BUILD_TYPE=Release ..
	cd ${BUILDDIR} && cpack -G DEB
	cd ${BUILDDIR} && cpack -G RPM

