obj-m := rpc_raspberry_sender.o
KDIR:=~/linux-rpi/
ARM:= ARCH=arm CROSS_COMPILE=/usr/bin/arm-linux-gnueabi-
PWD :=  $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) $(ARM) modules
	arm-linux-gnueabi-gcc rpc_raspberry_sender_lib.c rasp3.c -o rasp3 -lpthread
	arm-linux-gnueabi-gcc rpc_raspberry_sender_lib.c rasp2.c -o rasp2 -lpthread

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) $(ARM) clean
	rm $(obj-m:.o=)
scp:
	scp $(obj-m:.o=.ko) mknod.sh rasp3 pi@rasp3:~/
	ssh pi@rasp3
