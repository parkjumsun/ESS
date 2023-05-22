#include "rpc_raspberry_sender.h"
#include <string.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

int service_to_fd[MAX_CONNECTION];

int connect_to_rasp(char* ip_addr, int port_num){
    int fd = open("/dev/rpc_raspberry_sender", O_RDWR);
    if(fd < 0)
        return -1;
    struct socket_args socket_args_lib;
    strcpy(socket_args_lib.ip_addr, ip_addr);
    socket_args_lib.port_num = port_num;
    int service_num = ioctl(fd, CONNECT_TO_RASP, &socket_args_lib);
    service_to_fd[service_num] = fd;
    return service_num;
}

int disconnect_to_rasp(int service_num){
    return ioctl(service_to_fd[service_num], DISCONNECT_TO_RASP, &service_num);
}

int remote_gpio_request_one(int service_num, unsigned int gpio, unsigned long flags, char* lable){
    struct request_gpio_one_arg;
    request_gpio_one_arg.service_num = service_num;
    strcpy(request_gpio_one_arg.label, label);
    request_gpio_one_arg.gpio = gpio;
    request_gpio_one_arg.flags = flags;
    return ioctl(fd, REMOTE_GPIO_REQUEST, &request_gpio_one_arg);
}



int main(int argc, char const *argv[]){
    int service_num = connect_to_rasp("127.0.0.1", 700);
    disconnect_to_rasp(service_num);
    return 0;
}
