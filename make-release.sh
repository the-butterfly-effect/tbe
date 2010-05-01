#!/bin/bash

#make -C 3rdParty release

mv tbe.pro tbe.pro.old
cat tbe.pro.old | sed s/debug/release/ >tbe.pro

3rdParty/bin/qmake tbe.pro
make
3rdParty/bin/lupdate tbe.pro
3rdParty/bin/lrelease tbe.pro

