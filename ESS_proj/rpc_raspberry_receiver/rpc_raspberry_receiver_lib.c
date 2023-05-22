#include "rpc_raspberry_receiver.h"
#include <string.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>



int init_rpc(unsigned long portnum){
    int fd = open("/dev/rpc_raspberry_receiver", O_RDWR);
    if(fd < 0)
        return -1;
    unsigned long portnum_lib = portnum;
    ioctl(fd, INIT, &portnum_lib);
    return fd;
}

int wating_from_sender(int fd){
    return ioctl(fd, WAIT_CONNECTION, NULL);
}


int main(int argc, char const *argv[])
{   
    int fd = init_rpc(700);
    wating_from_sender(fd);
    return 0;
}
