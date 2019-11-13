obj-m  := ramdisk.o
obj-m  += ramskel.o
KDIR= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD)  modules
clean: 
	rm -rf $(wildcard *.o *.ko *.mo* Mo* *.order)
	

