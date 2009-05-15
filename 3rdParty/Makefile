ODE=ode-0.11


MYINCDIR=${PWD}/include
MYLIBDIR=${PWD}/libs



all:	ode

clean:
	rm -rf ${ODE}


mrproper: clean
	rm -rf ${MYINCDIR}/* ${MYLIBDIR}/*

ode:
	tar xjvf ${ODE}.tar.bz2
	cd ${ODE} && ./configure --disable-demos --enable-static --disable-shared --with-drawstuff=none --disable-ou --includedir=${MYINCDIR} --libdir=${MYLIBDIR} --bindir=${MYLIBDIR}
	make -C ${ODE}
	make -C ${ODE} install

