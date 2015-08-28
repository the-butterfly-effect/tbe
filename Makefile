# Makefile to 'simplify' using cmake
#
#

BUILDDIR=build
# if you want to build Release, just call "make BUILDTYPE=Release all"
BUILDTYPE=Debug

all: usr/games/tbe

usr/games/tbe: ${BUILDDIR}/src/tbe

${BUILDDIR}/src/tbe:
	mkdir -p ${BUILDDIR}
	cd ${BUILDDIR} && cmake -DCMAKE_BUILD_TYPE=${BUILDTYPE} ..
	cd ${BUILDDIR} && make -j 6
	cd ${BUILDDIR} && make DESTDIR=.. install
	ln -sf usr/games/tbe .

slow:
	cd ${BUILDDIR} && make
	cp ${BUILDDIR}/src/tbe usr/games

regression: usr/games/tbe
	./tbe --regression levels/draft/save-the-butterfly.xml:28,levels/draft/bouncing_balls.xml:10;levels/picnic/picnic-1.xml:10

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

