# Makefile to 'simplify' using cmake
#
#

BUILDDIR=build

all:
	mkdir -p ${BUILDDIR}
	cd ${BUILDDIR} && cmake ..
	cd ${BUILDDIR} && make
	cd ${BUILDDIR} && make translations
	cd ${BUILDDIR} && make DESTDIR=.. install

clean:
	cd ${BUILDDIR} && make clean

distclean:
	rm -rf ${BUILDDIR}

package: all
	cd ${BUILDDIR} && make package
