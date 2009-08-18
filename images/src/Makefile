all:
	for I in `ls -1p | grep '/'`; do echo $$I; make -C $$I all_image; done;

clean:
	for I in `ls -1p | grep '/'`; do echo $$I; make -C $$I all_clean; done;

all_image:
	cp ${SVGSRC} ../${SVGDST}

all_clean:
	rm -f ../${SVGDST}
	
