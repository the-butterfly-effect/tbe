#!/bin/bash
echo running installjammer now
unix2dos -n ../AUTHORS ../AUTHORS.txt
unix2dos -n ../COPYING ../COPYING.txt
unix2dos -n ../README  ../README.txt

if [ ! -f ../i18n/tbe_nl.qm ];
then 
	echo "Oops... No i18n files built!"
	exit;
else
	rm ../i18n/*.ts
fi

../3rdParty/bin/installjammer --verbose --platform Windows --build-dir tmp --build $PWD/TheButterflyEffect.mpi
