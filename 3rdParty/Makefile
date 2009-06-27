ODE=ode-0.11
BOX2D=Box2D_v2.0.1


MYINCDIR=${PWD}/include
MYLIBDIR=${PWD}/libs


# no longer build ode
all:	box2d

clean:
	rm -rf ${ODE}
	rm -rf ${BOX2D}

mrproper: clean
	rm -rf ${MYINCDIR}/* ${MYLIBDIR}/*

ode:
	tar xjvf ${ODE}.tar.bz2
	cd ${ODE} && ./configure --disable-demos --enable-static --disable-shared --with-drawstuff=none --disable-ou --includedir=${MYINCDIR} --libdir=${MYLIBDIR} --bindir=${MYLIBDIR}
	make -C ${ODE}
	make -C ${ODE} install

box2d:
	unzip ${BOX2D}.zip
	patch -p0 <box2d_compile.patch
	patch -p0 <box2d_fix_warnings.patch
	cd ${BOX2D}/Box2D/Source && make Gen/float/libbox2d.a 
	cp ${BOX2D}/Box2D/Source/Gen/float/libbox2d.a libs
	# not possible to extract includes - they're a mess
