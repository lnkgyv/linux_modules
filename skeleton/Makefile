CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)
DEST = /lib/modules/$(CURRENT)/misc

TARGET = skeleton
obj-m := $(TARGET).o

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	@rm -f *.ko *.mod *.o .*.cmd .*.flags *.mod.c *.order *.dwo
	@rm -f .*.*.cmd *.symvers *~ *.*~ TODO.* .*.dwo
	@rm -fR .tmp*
	@rm -rf .tmp_versions
