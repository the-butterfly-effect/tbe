all:
	@echo "This readme contains information on the origins of all files" >  README.all
	@echo "in this directory. Note that several files mentioned in the " >> README.all
	@echo "README may not be present in the release package."            >> README.all
	@echo "" >> README.all
	find . -name 'README' -exec cat {} >> README.all 2>/dev/null \;
	@mv README.all README
	@for I in `ls -1p | grep '/'`; do make -C $$I all; done;

clean:
	rm -f *.png *.svg README
