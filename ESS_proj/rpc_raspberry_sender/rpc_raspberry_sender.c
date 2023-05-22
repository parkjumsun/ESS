#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/cdev.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/vmalloc.h> 
#include <linux/inet.h>  
#include <linux/socket.h>  
#include <net/sock.h>  
#include <linux/in.h>  
#include "rpc_raspberry_sender.h"


#define DEV_NAME "rpc_raspberry_sender"

MODULE_LICENSE("GPL");

#define BUFFSIZE 1000


static dev_t dev_num;
static struct cdev* cd_cdev;
struct socket* sock[MAX_CONNECTION] = {NULL};
static int socketIdx = 0;
struct socket_args* kern_sock_arg;
struct kvec* send_vec;
struct kvec* recv_vec;
struct msghdr* send_msg;
struct msghdr* recv_msg;
char* send_byte_buffer;
char* recv_byte_buffer;




static int connect_to_rasp(struct socket_args* arg){
    struct socket_args temp_buf_socket;
    struct sockaddr_in s_addr;
    int ret;
    if(socketIdx >= MAX_CONNECTION){
        return -1;
    }
    // need to modify
    // need to implemetn lock
    copy_from_user(&temp_buf_socket, arg, sizeof(struct socket_args));
    memset(&s_addr,0,sizeof(s_addr));  
    s_addr.sin_family=AF_INET;
    s_addr.sin_port=htons(temp_buf_socket.port_num);  
    s_addr.sin_addr.s_addr=in_aton(temp_buf_socket.ip_addr);
    ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock[socketIdx]);
    if (ret < 0) {
        printk("rpc_raspberry_sender: socket create error!\n");
        ret = -1;
    }
    else{
        printk("rpc_raspberry_sender: socket create ok!\n");
        ret = sock[socketIdx]->ops->connect(sock[socketIdx], (struct sockaddr *)&s_addr, sizeof(s_addr), 0);
        if (ret != 0) {
            printk("rpc_raspberry_sender: connect error!\n");
            printk("%d\n",ret);
            ret = -1;
        }
        else{
            printk("rpc_raspberry_sender: connect ok!\n");
            ret = socketIdx;
            socketIdx++;
            *send_byte_buffer = 1;
        }
    }
    return ret;
}

static int disconnect_to_rasp(unsigned long* arg){
    int kern_service_idx;
    int ret;
    copy_from_user(&kern_service_idx, arg, sizeof(int));

    send_vec->iov_base = send_byte_buffer; 
    send_vec->iov_len = 1;
    recv_vec->iov_base = recv_byte_buffer; 
    recv_vec->iov_len = 1;  

    *send_byte_buffer = END_SIGNAL;
    ret = kernel_sendmsg(sock[kern_service_idx], send_msg, send_vec, 1, 1);
    if(ret < 0){
        printk("rpc_raspberry_sender: send error!!\n");
    }
    else{
        printk("rpc_raspberry_sender: send ok!!\n");
        ret = kernel_recvmsg(sock[kern_service_idx], recv_msg, recv_vec, 1, BUFFSIZE, 0);
        if(ret < 0){
            printk("rpc_raspberry_sender: receiv error!!\n");
        }
        else{
            printk("rpc_raspberry_sender: receive ok!!\n");
            ret = *recv_byte_buffer;
        }
    }
    printk("ret: %d\n", ret);

    return ret;
}

static int remote_request_gpio(struct request_gpio_one_arg* arg){
    struct request_gpio_one_arg kern_gpio_one_arg;
    copy_from_user(kern_gpio_one_arg, arg, sizeof(struct request_gpio_one_arg));
    printk("label: %s\n", kern_gpio_one_arg.label);
    send_vec->iov_base = send_byte_buffer; 
    send_vec->iov_len = 1;
    recv_vec->iov_base = recv_byte_buffer; 
    recv_vec->iov_len = 1;  

}


static int rpc_raspberry_sender_open(struct inode* inode, struct file* file){
    printk("rpc_raspberry_sender: open\n");
    return 0;
}

static int rpc_raspberry_sender_release(struct inode* inode, struct file* file){
    printk("rpc_raspberry_sender: release\n");
    return 0;
}

static long rpc_raspberry_sender_ioctl(struct file* file, unsigned int cmd, unsigned long arg){
    int ret = 0;
    struct socket_args* s_arg;
    struct request_gpio_one_arg* rgo_arg;
    unsigned long* service_idx;
    printk("rpc_raspberry_sender: IOCTL\n");
    switch(cmd){
        case CONNECT_TO_RASP:
            s_arg = (struct socket_args*)arg;
            ret = connect_to_rasp(s_arg);
            break;
        case DISCONNECT_TO_RASP:
            service_idx = (unsigned long*)arg;
            ret = disconnect_to_rasp(service_idx);
        case REMOTE_GPIO_REQUEST:
            rgo_arg = (struct request_gpio_one_arg*)arg;
            ret = remote_request_to_gpio(rgo_arg);

    }
    return ret;
}


struct file_operations rpc_raspberry_sender_ops = {
    .open = rpc_raspberry_sender_open,
    .release = rpc_raspberry_sender_release,
    .unlocked_ioctl = rpc_raspberry_sender_ioctl
};

static int __init rpc_raspberry_sender_init(void){
    printk("rpc_raspberry_sender: Init Module\n");
    alloc_chrdev_region(&dev_num, 0, 1, DEV_NAME);
    cd_cdev = cdev_alloc();
    cdev_init(cd_cdev, &rpc_raspberry_sender_ops);
    cdev_add(cd_cdev, dev_num, 1);

    send_vec = kmalloc(sizeof(struct kvec), GFP_KERNEL);
    recv_vec = kmalloc(sizeof(struct kvec), GFP_KERNEL);
    send_msg = kmalloc(sizeof(struct msghdr), GFP_KERNEL);
    recv_msg = kmalloc(sizeof(struct msghdr), GFP_KERNEL);
    send_byte_buffer = kmalloc(sizeof(char), GFP_KERNEL);
    recv_byte_buffer = kmalloc(sizeof(char), GFP_KERNEL);
    memset(send_vec, 0, sizeof(struct kvec));
    memset(recv_vec, 0, sizeof(struct kvec));
    memset(send_msg, 0, sizeof(struct msghdr));
    memset(recv_msg, 0, sizeof(struct msghdr));
    memset(send_byte_buffer, 0, sizeof(char));
    memset(recv_byte_buffer, 0, sizeof(char));
    recv_msg->msg_flags = MSG_NOSIGNAL;    

    return 0;
}

static void __exit rpc_raspberry_sender_exit(void){
    int i = 0;
    printk("rpc_raspberry_sender: Exit Module\n");
    cdev_del(cd_cdev);
    unregister_chrdev_region(dev_num, 1);
    for(i = 0; i < MAX_CONNECTION; i++){
        if(sock[i] != NULL){
            sock_release(sock[i]);
        }
    }
    kfree(send_vec);
    kfree(recv_vec);
    kfree(send_msg);
    kfree(recv_msg);
    kfree(send_byte_buffer);
    kfree(recv_byte_buffer);
}

module_init(rpc_raspberry_sender_init);
module_exit(rpc_raspberry_sender_exit);






