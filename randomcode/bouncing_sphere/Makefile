MYINCDIR=../../3rdParty/include
MYLIBDIR=../../3rdParty/libs

all:
	g++ -o bbounce 2bounce_box2d.cpp -I${MYINCDIR} -L${MYLIBDIR} -lode
	#g++ -o bounce bouncingsphereexample_no_drawstuff.c -I${MYINCDIR} -L${MYLIBDIR} -lode
	#g++ -o 2bounce 2bouncing_no_draw.cpp -I${MYINCDIR} -L${MYLIBDIR} -lode
	
clean:
	rm -f bbounce 2bounce bounce *~
