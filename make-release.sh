#!/bin/bash

#QMAKE=`which qmake`
QMAKE=3rdParty/bin/qmake

QT4DIR=`echo $QMAKE | sed 's/\/qmake//'`

(cd 3rdParty && make release)

mv tbe.pro tbe.pro.old
cat tbe.pro.old | sed 's/debug/release link_prl/' >tbe.pro

${QMAKE} tbe.pro
make clean
make
strip tbe
${QT4DIR}/lupdate tbe.pro
${QT4DIR}/lrelease tbe.pro

