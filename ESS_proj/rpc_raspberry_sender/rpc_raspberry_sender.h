#define MAX_CONNECTION 10

#define IOCTL_START_NUM 0x80
#define IOCTL_NUM1 IOCTL_START_NUM + 1
#define IOCTL_NUM2 IOCTL_START_NUM + 2
#define IOCTL_NUM3 IOCTL_START_NUM + 3
#define IOCTL_NUM4 IOCTL_START_NUM + 4
#define IOCTL_NUM5 IOCTL_START_NUM + 5
#define SIMPLE_IOCTL_NUM 'z'


#define CONNECT_TO_RASP _IOWR(SIMPLE_IOCTL_NUM, IOCTL_NUM1, unsigned long*)
#define DISCONNECT_TO_RASP _IOWR(SIMPLE_IOCTL_NUM, IOCTL_NUM2, unsigned long*)
#define REMOTE_GPIO_REQUEST _IOWR(SIMPLE_IOCTL_NUM, IOCTL_NUM3, unsigned long*)
#define REMOTE_GPIO_WRITE _IOWR(SIMPLE_IOCTL_NUM, IOCTL_NUM4, unsigned long*)
#define REMOTE_GPIO_READ _IOWR(SIMPLE_IOCTL_NUM, IOCTL_NUM5, unsigned long*)




#define GPIO_REQUEST_SIGNAL 10
#define GPIO_REQUEST_WRITE 11
#define GPIO_REQUEST_READ 12

#define END_SIGNAL 100

struct socket_args {
    char ip_addr[20];
    int port_num;
};

struct request_gpio_one_arg{
    int service_num;
    char label[20];
    unsigned int gpio;
    unsigned int flags;
};