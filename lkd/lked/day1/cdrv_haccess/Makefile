obj-m := rtc.o
obj-m += rtc_chrdev.o
obj-m += rtcproc.o
obj-m += rtc_ioctl.o

all:
	make -C /lib/modules/$(shell uname -r)/build SUBDIRS=$(PWD) modules


clean:
	rm -rf *.o *.order *.mod.c *.symvers  *.ko

