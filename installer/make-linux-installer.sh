#!/bin/bash
echo running installjammer now
../3rdParty/bin/installjammer --verbose --build-dir tmp --platform Linux-x86 --build $PWD/TheButterflyEffect.mpi
../3rdParty/bin/installjammer --verbose --build-dir tmp --platform Linux-x86_64 --build $PWD/TheButterflyEffect.mpi
