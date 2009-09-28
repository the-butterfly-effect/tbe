all:
	@for I in `ls -1p | grep '/'`; do make -C $$I all_image; done;

clean:
	@for I in `ls -1p | grep '/'`; do make -C $$I all_clean; done;

all_image:
	@if [ -n "${SVGDST}" ]; \
	then \
		cp ${SVGSRC} ../${SVGDST}; \
	else \
		for I in "${SVGSRC}"; do cp $$I ..; done; \
	fi;

all_clean:
	if [ -n "${SVGDST}" ]; \
	then \
		rm -f ../${SVGDST}; \
	else \
		cd .. && rm -f ${SVGSRC}; \
	fi;
	
