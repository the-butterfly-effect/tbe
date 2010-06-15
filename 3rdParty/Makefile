BOX2D=Box2D_v2.0.1
QTVERSION=qt-x11-opensource-src-4.5.3
INSTALLJAMMER=InstallJammer-1.2.14-Linux-x86-Install
WINBOX2D=WinBox2D

MY3RDPARTYDIR=${PWD}
MYINCDIR=${MY3RDPARTYDIR}/include
MYLIBDIR=${MY3RDPARTYDIR}/lib
MYBINDIR=${MY3RDPARTYDIR}/bin
MYMKSPECDIR=${MY3RDPARTYDIR}/mkspecs
MYPLUGINSDIR=${MY3RDPARTYDIR}/plugins

# this will allow 3 parallel build threads - works great on a Core2 Duo :-)
FASTERMAKE=-j3

# only build static box2d / use Qt from host system
all:	${MYLIBDIR}/libbox2d.a 

# only build (static) QT for release builds
release: ${MYLIBDIR}/libbox2d.a ${MYLIBDIR}/libQtCore.a installjammer

# clean all build artifacts
clean:
	rm -rf ${BOX2D}
	rm -rf ${QTVERSION}
	rm -rf ${MYINCDIR}/* ${MYLIBDIR}/* ${MYBINDIR}/* ${MYPLUGINSDIR}/* ${MYMKSPECDIR}/*
	rm -rf installjammer
	rm -rf ${WINBOX2D} ${WINBOX2D}.zip

#----------------------------- internal targets

${MYLIBDIR}/libbox2d.a:
	unzip -q ${BOX2D}.zip
	patch -p0 <box2d_compile.patch
	patch -p0 <box2d_fix_warnings.patch
	cd ${BOX2D}/Box2D/Source && make ${FASTERMAKE} Gen/float/libbox2d.a 
	cp ${BOX2D}/Box2D/Source/Gen/float/libbox2d.a ${MYLIBDIR}

# because I don't have a shell environment on my windows box, I need to
# simplify my directory structure :-(
# here we create a zip archive with all flattened files
${WINBOX2D}.zip: ${MYLIBDIR}/libbox2d.a
	mkdir -p ${WINBOX2D}
	find ${BOX2D}/Box2D/Source -name '*.cpp' > ${WINBOX2D}/manifest
	find ${BOX2D}/Box2D/Source -name '*.h'  >> ${WINBOX2D}/manifest
	echo "SOURCES= \\" > ${WINBOX2D}/Makefile
	for I in `cat ${WINBOX2D}/manifest`; \
		do \
		FULLNAME=`echo $$I | sed 's|.*/||'`; \
		#echo I $$I full $${FULLNAME}; \
		cat $$I | sed 's|#include ".*/\(.*\.h\)"|#include "\1"|' >${WINBOX2D}/$${FULLNAME}; \
		echo "$${FULLNAME} \\" >> ${WINBOX2D}/Makefile; \
	done;
	/bin/echo -e '\nlibbox2d.a: $$(SOURCES:.cpp=.o)\n\tar cr $$@ $$^\n\tranlib $$@' >> ${WINBOX2D}/Makefile
	/bin/echo -e '\n%.o: %.cpp\n\tc++ -O2 -c -o $$@ $$<' >> ${WINBOX2D}/Makefile
	zip ${WINBOX2D}.zip ${WINBOX2D}/*
	rm -rf ${WINBOX2D}


# QT is build statically, with as little options as reasonably possible
# we also specifically exclude the graphical developer tools 
# (designer/linguist/assistant) from the build to save time
${MYLIBDIR}/libQtCore.a: ${QTVERSION}.tar.gz
	tar xzf ${QTVERSION}.tar.gz
	mv ${QTVERSION}/tools/tools.pro ${QTVERSION}/tools/tools.pro.old
	cat ${QTVERSION}/tools/tools.pro.old | sed s/designer// \
		| sed s/assistant// > ${QTVERSION}/tools/tools.pro
	mv ${QTVERSION}/tools/linguist/linguist.pro ${QTVERSION}/tools/linguist/linguist.pro.old
	cat ${QTVERSION}/tools/linguist/linguist.pro.old | grep -v linguist \
		> ${QTVERSION}/tools/linguist/linguist.pro
	cd ${QTVERSION} && echo "yes" | ./configure -opensource -static\
           -exceptions -silent -no-largefile\
           -no-gif -no-libtiff -no-libmng -no-openssl -no-nis -no-cups \
           -no-iconv -no-pch -no-dbus -no-nas-sound \
           -nomake examples -nomake demos -nomake docs \
           -prefix ${MY3RDPARTYDIR}
	cd ${QTVERSION} && make ${FASTERMAKE}
	cd ${QTVERSION} && make install


${QTVERSION}.tar.gz:
	curl --remote-name ftp://ftp.qt.nokia.com/qt/source/${QTVERSION}.tar.gz


${INSTALLJAMMER}:
	curl --remote-name http://downloads.installjammer.com/installjammer/1.2/${INSTALLJAMMER}

installjammer: ${INSTALLJAMMER}
	chmod u+x ${INSTALLJAMMER};
	/bin/echo -e "Y\n\n" | ./${INSTALLJAMMER} --mode console --response-file install-jammer-answers;
	/bin/echo -e 'IJLOCATION=`echo $$0 | sed s^/installjammer$$^^`/../installjammer/installjammer\ncd $${IJLOCATION} && ./installjammer $$@' >${MYBINDIR}/installjammer;
	chmod u+x ${MYBINDIR}/installjammer;

