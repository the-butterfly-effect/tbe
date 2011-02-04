BOX2D=Box2D_v2.1.2
SRCBOX2D=../src/box2d
QTVERSION=qt-x11-opensource-src-4.5.3
INSTALLJAMMER=InstallJammer-1.2.14-Linux-x86-Install

MY3RDPARTYDIR=${PWD}
MYINCDIR=${MY3RDPARTYDIR}/include
MYLIBDIR=${MY3RDPARTYDIR}/lib
MYBINDIR=${MY3RDPARTYDIR}/bin
MYMKSPECDIR=${MY3RDPARTYDIR}/mkspecs
MYPLUGINSDIR=${MY3RDPARTYDIR}/plugins

# this will allow 3 parallel build threads - works great on a Core2 Duo :-)
FASTERMAKE=-j3

all:	 

# only build (static) QT for release builds
release: ${MYLIBDIR}/libQtCore.a installjammer

# clean all build artifacts
clean:
	rm -rf ${BOX2D}
	rm -rf ${QTVERSION}
	rm -rf ${MYINCDIR}/* ${MYLIBDIR}/* ${MYBINDIR}/* ${MYPLUGINSDIR}/* ${MYMKSPECDIR}/*
	rm -rf installjammer

#----------------------------- internal targets

${BOX2D}.zip:
	curl --remote-name http://box2d.googlecode.com/files/${BOX2D}.zip

${BOX2D}:
	unzip ${BOX2D}.zip

# in order to be able to update 
${SRCBOX2D}: ${BOX2D}.zip ${BOX2D}
	mkdir -p ${SRCBOX2D}
	rm -f ${BOX2D}/manifest
	find ${BOX2D}/Box2D/Box2D -name '*.cpp' > ${BOX2D}/manifest
	find ${BOX2D}/Box2D/Box2D -name '*.h'  >> ${BOX2D}/manifest
	for I in `cat ${BOX2D}/manifest`; \
		do \
		FULLNAME=`echo $$I | sed 's|.*/||'`; \
		#echo I $$I full $${FULLNAME}; \
		cat $$I | sed 's|#include <.*/\(.*\.h\)>|#include "\1"|' >${SRCBOX2D}/$${FULLNAME}; \
	done;


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
	cd ${QTVERSION} && ${MAKE} ${FASTERMAKE}
	cd ${QTVERSION} && ${MAKE} install


${QTVERSION}.tar.gz:
	curl --remote-name ftp://ftp.qt.nokia.com/qt/source/${QTVERSION}.tar.gz


${INSTALLJAMMER}:
	curl --remote-name http://downloads.installjammer.com/installjammer/1.2/${INSTALLJAMMER}

installjammer: ${INSTALLJAMMER}
	chmod u+x ${INSTALLJAMMER};
	/bin/echo -e "Y\n\n" | ./${INSTALLJAMMER} --mode console --response-file install-jammer-answers;
	/bin/echo -e 'IJLOCATION=`echo $$0 | sed s^/installjammer$$^^`/../installjammer/installjammer\ncd $${IJLOCATION} && ./installjammer $$@' >${MYBINDIR}/installjammer;
	chmod u+x ${MYBINDIR}/installjammer;

