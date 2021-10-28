obj-m += hidefiles.o

hidefiles-y := main.o syscalls.o functs.o list.o 
KDIR=/lib/modules/$(shell uname -r)/build 

all:
	make -C ${KDIR} M=$(PWD) modules

clean:
	make -C ${KDIR}  M=$(PWD) clean
