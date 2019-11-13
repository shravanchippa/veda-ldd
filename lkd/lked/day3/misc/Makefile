obj-m :=timer.o
obj-m +=intthread.o
obj-m +=inttest.o
obj-m +=uiostub.o
obj-m +=mytasklet.o
obj-m +=interrupt.o
obj-m +=taskletcount.o
obj-m +=taskletqueue.o

KDIR= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean: 
	 rm -rf $(wildcard *.o *.ko *.mod.* .c* .t* *.cmd test Module.symvers *.order *.markers)
