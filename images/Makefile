all:
	@echo "This readme contains information on the origins of all files" >  README
	@echo "in this directory. Note that several files mentioned in the " >> README
	@echo "README may not be present in the release package."            >> README
	@echo "" >> README
	find . -name 'README' -exec cat {} >> README \;
	@for I in `ls -1p | grep '/'`; do make -C $$I all; done;

clean:
	rm -f *.png *.svg README
