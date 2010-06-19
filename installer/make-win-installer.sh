#!/bin/bash
echo running installjammer now
unix2dos -n ../AUTHORS ../AUTHORS.txt
unix2dos -n ../COPYING ../COPYING.txt
unix2dos -n ../README  ../README.txt

../3rdParty/bin/installjammer --verbose --platform Windows --build-dir tmp --build $PWD/TheButterflyEffect.mpi
