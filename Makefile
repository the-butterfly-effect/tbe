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
	rm -rf ${BUILDDIR}

lang:
	 lupdate -recursive src -ts i18n/tbe_*.ts
	 lrelease -compress i18n/*.ts 
