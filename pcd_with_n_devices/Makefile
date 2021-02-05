obj-m := pcd_n.o

KDIR := /lib/modules/5.8.0-40-generic/build/
PWD := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean

