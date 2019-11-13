#obj-m := lcd_module.o
obj-m += s3c2410fb.o


ARCH:=arm
CROSS_COMPILE:=arm-linux-
#KDIR :=/embedded/arm9/mini2440/linux-3.9.8/
#KDIR :=/root/Desktop/linux-2.6.37.2
KDIR :=/root/Desktop/backup/linux-3.10.4
#KDIR := /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)
default:
         
	 $(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
        
	  $(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean


















#all:
        
#	     make  -C  /embedded/arm9/linux-2.6.37.2/ M=$(PWD) modules 
	#make -C  /lib/modules/$(shell uname -r)/build M=$(PWD) modules
#clean:

 #        make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
