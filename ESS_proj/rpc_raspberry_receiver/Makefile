obj-m := rpc_raspberry_receiver.o
KDIR:=~/linux-rpi/
ARM:= ARCH=arm CROSS_COMPILE=/usr/bin/arm-linux-gnueabi-
PWD :=  $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) $(ARM) modules
	arm-linux-gnueabi-gcc rpc_raspberry_receiver_lib.c -o rpc_raspberry_receiver
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) $(ARM) clean
	rm $(obj-m:.o=)
scp:
	scp $(obj-m:.o=.ko) mknod.sh $(obj-m:.o=) pi@rasp1:~/
	ssh pi@rasp1
