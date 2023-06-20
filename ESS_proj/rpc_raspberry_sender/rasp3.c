#include "rpc_raspberry_sender_lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define SWITCH1 13
#define LED1 6
#define LED2 13
#define LED3 19
#define LED4 26

#define SWITCH2 19
#define SWITCH3 26

// user area

void my_func(int service_num){
    int val1 = local_gpio_get_value(service_num, LED1);
    int val2 = local_gpio_get_value(service_num, LED2);
    int val3 = local_gpio_get_value(service_num, LED3);
    int val4 = local_gpio_get_value(service_num, LED4);

    local_gpio_set_value(service_num, LED1, !val1);
    local_gpio_set_value(service_num, LED2, !val2);
    local_gpio_set_value(service_num, LED3, !val3);
    local_gpio_set_value(service_num, LED4, !val4);
    printf("test\n");

}

int main(int argc, char const *argv[]){
    int service_num = connect_to_rasp("172.21.114.61", 700);
    int end;
    remote_gpio_request_one(service_num, SWITCH1, REMOTE_GPIOF_IN, "test1");
    // remote_gpio_request_one(service_num, SWITCH2, REMOTE_GPIOF_IN, "test2");
    // remote_gpio_request_one(service_num, SWITCH3, REMOTE_GPIOF_IN, "test3");
    remote_request_irq(service_num, 800, SWITCH1, my_func, REMOTE_IRQF_TRIGGER_RISING, "test4", "172.21.114.149");
    // remote_request_irq(service_num, 800, SWITCH2, my_func, REMOTE_IRQF_TRIGGER_RISING, "test5", "192.168.87.149");
    // remote_request_irq(service_num, 800, SWITCH3, my_func, REMOTE_IRQF_TRIGGER_RISING, "test6", "192.168.87.149");
    
    local_gpio_request_one(service_num, LED1, REMOTE_GPIOF_OUT_INIT_LOW, "test5");
    local_gpio_request_one(service_num, LED2, REMOTE_GPIOF_OUT_INIT_LOW, "test6");
    local_gpio_request_one(service_num, LED3, REMOTE_GPIOF_OUT_INIT_LOW, "test7");
    local_gpio_request_one(service_num, LED4, REMOTE_GPIOF_OUT_INIT_LOW, "test7");
    

    scanf("%d", &end);
    
    remote_free_irq(service_num, SWITCH1);
    // remote_free_irq(service_num, SWITCH2);
    // remote_free_irq(service_num, SWITCH3);
    remote_gpio_free(service_num, SWITCH1);
    // remote_gpio_free(service_num, SWITCH2);
    // remote_gpio_free(service_num, SWITCH3);
    local_gpio_free(service_num, LED1);
    local_gpio_free(service_num, LED2);
    local_gpio_free(service_num, LED3);
    local_gpio_free(service_num, LED4);
    disconnect_to_rasp(service_num);
    return 0;
}
