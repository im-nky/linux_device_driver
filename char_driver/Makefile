
obj-m := char.o

KDIR := /lib/modules/4.15.0-72-generic/build/
PWD := $(shell pwd)


all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean

