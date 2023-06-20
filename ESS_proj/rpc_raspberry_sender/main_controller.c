
#include "rpc_raspberry_sender_lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
// user area

void my_func1(int service_num){
    printf("interrupt receive\n");
}
int main(int argc, char const *argv[]){
    int service_num = connect_to_rasp("192.168.87.61", 700);
    remote_gpio_request_one(service_num, 12, REMOTE_GPIOF_IN, "test1");
    remote_request_irq(service_num, 800, 12, my_func1, REMOTE_IRQF_TRIGGER_RISING, "test3", "192.168.87.25");
    sleep(30);
    remote_free_irq(service_num, 12);
    remote_gpio_free(service_num, 12);
    disconnect_to_rasp(service_num);
    return 0;
}
