obj-m += hidefiles.o

hidefiles-y := main.o syscalls.o functs.o list.o 


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
