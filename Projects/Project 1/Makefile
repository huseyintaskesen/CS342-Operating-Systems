all: phistogram thistogram


phistogram: phistogram.c 
	gcc -o phistogram phistogram.c -lm
thistogram: thistogram.c 
	gcc -o thistogram thistogram.c -lm -lpthread

# thistogram: thistogram.c 
#  	gcc -o thistogram thistogram.c -lpthread -lm

clean:
	/bin/rm -fr *~ *.o phistogram thistogram