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

sanitizers:	EXTRACMAKEFLAGS=-DECM_ENABLE_SANITIZERS='address;leak;undefined' -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_LINKER=clang++
sanitizers:     ${BUILDDIR}/src/tbe

${BUILDDIR}/src/tbe:
	mkdir -p ${BUILDDIR}
	cd ${BUILDDIR} && cmake ${EXTRACMAKEFLAGS} -DCMAKE_BUILD_TYPE=${BUILDTYPE} -DWITH_DOCS=${WITH_DOCS} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
	cd ${BUILDDIR} && make -j 6
	cd ${BUILDDIR} && make DESTDIR=.. install
	ln -sf usr/games/tbe .

slow:
	cd ${BUILDDIR} && make
	cp ${BUILDDIR}/src/tbe usr/games

regression: usr/games/tbe
	./tbe --regression levels/draft/balloons-do-poof.xml:6, \
	      --regression levels/draft/bouncing_balls.xml:6, \
	      --regression levels/draft/butterfly-on-steroids.xml:65, \
	      --regression levels/draft/cola-powered-bike.v2.xml:20, \
              --regression=levels/draft/construction_yard.xml:26, \
              --regression levels/draft/contraption1.xml:36, \
              --regression levels/draft/float-balloon-float.xml:21, \
	      --regression levels/draft/geyser.xml:16, \
	      --regression levels/draft/house_of_cards.xml:20, \
	      --regression=levels/draft/in_the_attic.xml:17, \
	      --regression levels/draft/jumping_around-2.xml:10, \
	      --regression=levels/draft/loopings2.xml:22, \
	      --regression levels/draft/save-the-butterfly.xml:28, \
	      --regression levels/draft/spare-the-balloon.xml:25, \
	      --regression levels/draft/the_ball_the_box_and_the_penguin.xml:32, \
	      --regression levels/angry/birds1.xml:13, \
	      --regression levels/elce09/002.xml:30, \
	      --regression levels/elce09/003.xml:40, \
	      --regression levels/elce09/004.xml:15, \
	      --regression levels/elce09/006.xml:8, \
	      --regression levels/games/pingus-1.xml:65, \
	      --regression levels/jumpingjack/party-at-office.xml:15, \
	      --regression levels/picnic/picnic-0.xml:13, \
	      --regression levels/picnic/picnic-1.xml:13, \
	      --regression=levels/picnic/picnic-3.xml:70

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

