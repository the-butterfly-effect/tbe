all:
	@for I in `ls -1p | grep '/'`; do make -C $$I all; done;

clean:
	rm -f *.png *.svg *.jpg README
