obj-m := rpc_raspberry_receiver.o
KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD :=  $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc rpc_raspberry_receiver_lib.c -o rpc_raspberry_receiver
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

	