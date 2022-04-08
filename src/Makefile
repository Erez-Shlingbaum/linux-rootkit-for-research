obj-m += hidefiles.o

hidefiles-y := main.o syscall_hooks.o
KDIR=/lib/modules/$(shell uname -r)/build 

ccflags-y := -std=gnu11 -Wno-declaration-after-statement -Wno-error=strict-prototypes -Wno-strict-prototypes

SRC = $(PWD)

all:
	make -C ${KDIR} M=$(SRC) modules

clean:
	make -C ${KDIR}  M=$(SRC) clean