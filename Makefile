# Makefile to 'simplify' using cmake
#
#

BUILDDIR=build

all:
	mkdir -p ${BUILDDIR}
	cd ${BUILDDIR} && cmake ..
	cd ${BUILDDIR} && make -j 6
	cd ${BUILDDIR} && make DESTDIR=.. install
	ln -s usr/local/bin/tbe .

slow:
	cd ${BUILDDIR} && make
	cp ${BUILDDIR}/src/tbe usr/local/bin

clean:
	rm -rf ${BUILDDIR} 

distclean:
	rm -rf ${BUILDDIR} usr CPack* CMakeFiles cmake_install.cmake CMakeCache.txt CMakeLists.txt.user
	find . -name '*~' -exec rm {} \;

package:
	# build STGZ and TGZ
	cd ${BUILDDIR} && cmake ..
	cd ${BUILDDIR} && make package
	cd ${BUILDDIR} && RPM=1 cmake ..
	cd ${BUILDDIR} && make package

