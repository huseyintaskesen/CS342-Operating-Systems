obj-m += project3.o app.c

all:	
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -g -Wall -o app app.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	$(RM) app
