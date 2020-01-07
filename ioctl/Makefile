# This assignment states that there is one module to be built from object file hello.o

obj-m += ioctl.o

KDIR := /lib/modules/$(shell uname -r)/build/
PWD := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
