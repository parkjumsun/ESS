#include "rpc_raspberry_sender_lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
// user area

void my_func1(int service_num){
    // int val = local_gpio_get_value(service_num, 17);
    int val = local_gpio_get_value(service_num, 17);
    local_gpio_set_value(service_num, 17, !val);
    printf("myfunc1_val : %d\n", val);

}

void my_func2(int service_num){
    int val = local_gpio_get_value(service_num, 17);
    local_gpio_set_value(service_num, 17, !val);
    printf("myfunc2_val : %d\n", val);
}

int main(int argc, char const *argv[]){
    int service_num = connect_to_rasp("192.168.87.61", 700);
    remote_gpio_request_one(service_num, 12, REMOTE_GPIOF_IN, "test1");
    remote_gpio_request_one(service_num, 20, REMOTE_GPIOF_IN, "test2");
    remote_request_irq(service_num, 800, 12, my_func1, REMOTE_IRQF_TRIGGER_RISING, "test3", "192.168.87.63");
    remote_request_irq(service_num, 800, 20, my_func2, REMOTE_IRQF_TRIGGER_RISING, "test4", "192.168.87.63");
    local_gpio_request_one(service_num, 17, REMOTE_GPIOF_OUT_INIT_LOW, "test5");
    sleep(30);
    remote_free_irq(service_num, 12);
    remote_free_irq(service_num, 20);
    remote_gpio_free(service_num, 12);
    remote_gpio_free(service_num, 20);
    local_gpio_free(service_num, 17);
    disconnect_to_rasp(service_num);
    return 0;
}
