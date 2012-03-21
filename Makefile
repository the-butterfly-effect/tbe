# Makefile to 'simplify' using cmake
#
#

BUILDDIR=build

all:
	mkdir -p ${BUILDDIR}
	cd ${BUILDDIR} && cmake ..
	cd ${BUILDDIR} && make
	cd ${BUILDDIR} && make DESTDIR=.. install

clean:
	cd ${BUILDDIR} && make clean

distclean:
	rm -rf ${BUILDDIR} usr CPack* CMakeFiles cmake_install.cmake CMakeCache.txt CMakeLists.txt.user
	find . -name '*~' -exec rm {} \;

package:
	# build STGZ and TGZ
	cd ${BUILDDIR} && cmake ..
	cd ${BUILDDIR} && make package
	cd ${BUILDDIR} && RPM=1 cmake ..
	cd ${BUILDDIR} && make package

