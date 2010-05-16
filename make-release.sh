#!/bin/bash

#QMAKE=`which qmake`
QMAKE=3rdParty/bin/qmake

(cd 3rdParty && make release)

mv tbe.pro tbe.pro.old
cat tbe.pro.old | sed 's/debug/release link_prl/' >tbe.pro

${QMAKE} tbe.pro
make clean
make
strip tbe
lupdate tbe.pro
lrelease tbe.pro

