# Makefile to 'simplify' using cmake
#
#

BUILDDIR=build
# if you want to build Release, just call "make BUILDTYPE=Release all"
BUILDTYPE=Debug
# if you want to build without docs, just call "make WITH_DOCS=off all"
WITH_DOCS=on

all: usr/games/tbe

usr/games/tbe: ${BUILDDIR}/src/tbe

${BUILDDIR}/src/tbe:
	mkdir -p ${BUILDDIR}
	cd ${BUILDDIR} && cmake -DCMAKE_BUILD_TYPE=${BUILDTYPE} -DWITH_DOCS=${WITH_DOCS} ..
	cd ${BUILDDIR} && make -j 6
	cd ${BUILDDIR} && make DESTDIR=.. install
	ln -sf usr/games/tbe .

slow:
	cd ${BUILDDIR} && make
	cp ${BUILDDIR}/src/tbe usr/games

regression: usr/games/tbe
	./tbe --regression levels/draft/bouncing_balls.xml:10, \
	      --regression levels/draft/geyser.xml:16, \
	      --regression levels/draft/jumping_around-2.xml:10, \
	      --regression levels/draft/save-the-butterfly.xml:28, \
	      --regression levels/angry/birds1.xml:10, \
	      --regression levels/elce09/002.xml:30, \
	      --regression levels/elce09/003.xml:40, \
	      --regression levels/elce09/004.xml:15, \
	      --regression levels/elce09/006.xml:8, \
	      --regression levels/picnic/picnic-0.xml:13, \
	      --regression levels/picnic/picnic-1.xml:13, \
	      --regression levels/draft/butterfly-on-steroids.xml:75

# levels currently known to fail regression:
# TODO: must be fixed before release!!!
failregression: usr/games/tbe
	./tbe --regression levels/draft/ballons-do-poof.xml:10

clean:
	rm -rf ${BUILDDIR} ./tbe usr

distclean:
	rm -rf ${BUILDDIR} usr CPack* CMakeFiles cmake_install.cmake CMakeCache.txt CMakeLists.txt.user ./tbe
	find . -name '*~' -exec rm {} \;

package:
	# build STGZ and TGZ
	cd ${BUILDDIR} && cmake ..
	cd ${BUILDDIR} && make package
	cd ${BUILDDIR} && RPM=1 cmake ..
	cd ${BUILDDIR} && make package

