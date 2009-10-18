BOX2D=Box2D_v2.0.1


MYINCDIR=include
MYLIBDIR=libs


all:	libs/libbox2d.a

clean:
	rm -rf ${BOX2D}
	rm -rf ${MYINCDIR}/* ${MYLIBDIR}/*

libs/libbox2d.a:
	unzip ${BOX2D}.zip
	patch -p0 <box2d_compile.patch
	patch -p0 <box2d_fix_warnings.patch
	cd ${BOX2D}/Box2D/Source && make Gen/float/libbox2d.a 
	cp ${BOX2D}/Box2D/Source/Gen/float/libbox2d.a libs
