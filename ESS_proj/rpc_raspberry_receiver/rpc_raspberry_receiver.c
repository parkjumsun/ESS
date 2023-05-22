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
#include <linux/in.h>  
#include <linux/inet.h>  
#include <linux/socket.h>  
#include <net/sock.h>  
#include "rpc_raspberry_receiver.h"

#define DEV_NAME "rpc_raspberry_receiver"

MODULE_LICENSE("GPL");



struct socket* sock = NULL;
struct socket* connection_sock[MAX_CONNECTION] = { NULL };


static dev_t dev_num;
static struct cdev* cd_cdev;

struct kvec* send_vec[MAX_CONNECTION];
struct kvec* recv_vec[MAX_CONNECTION];
struct msghdr* send_msg[MAX_CONNECTION];
struct msghdr* recv_msg[MAX_CONNECTION];
char* send_byte_buffer[MAX_CONNECTION];
char* recv_byte_buffer[MAX_CONNECTION];
static unsigned long socketIdx = 0;

struct task_struct* main_task[MAX_CONNECTION] = { NULL };





static int init_socket(unsigned long* arg){
    unsigned long portnum;
    struct sockaddr_in s_addr;
    int ret = 1;
    copy_from_user(&portnum, arg, sizeof(unsigned long));
    s_addr.sin_family=AF_INET;
    s_addr.sin_port=htons(portnum);  
    s_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock); 
    if(ret){  
        printk("rpc_raspberry_receiver: socket_create error!\n");  
        ret = -1;
    }
    else{
        printk("rpc_raspberry_receiver: socket_create ok!\n");
        ret=kernel_bind(sock,(struct sockaddr *)&s_addr, sizeof(struct sockaddr_in));  
        if(ret<0){  
            printk("rpc_raspberry_receiver: bind error\n");  
            ret = -1;
        }
        else{            
            printk("rpc_raspberry_receiver: bind ok!\n");  
            ret=kernel_listen(sock,10);  
            if(ret<0){  
                printk("rpc_raspberry_receiver: listen error\n");  
                ret = -1;  
            }  
            else{
                printk("rpc_raspberry_receiver: listen ok!\n"); 
                ret = 1;
            }    
        }
    }
    return ret;
}

int mainHandler(void* arg){
    int ret;
    unsigned long idx = (unsigned long)arg;
    while(true){
        send_vec[idx]->iov_base = send_byte_buffer[idx]; 
        send_vec[idx]->iov_len = 1;
        recv_vec[idx]->iov_base = recv_byte_buffer[idx]; 
        recv_vec[idx]->iov_len = 1;  
        *send_byte_buffer[idx] = 2;
        ret = kernel_recvmsg(connection_sock[idx], recv_msg[idx], recv_vec[idx], 1, 1, 0);
        if(ret < 0){
            printk("rpc_raspberry_receiver: receive error!!\n");
        }
        else{
            printk("rpc_raspberry_receiver: reiceive ok!!\n");
            printk("receiev: %d", *recv_byte_buffer[idx]);
            if(*recv_byte_buffer[idx] == GPIO_REQUEST_READ){
                // request register gpio
                printk("GPIO_REQUEST\n");
            }
            if(*recv_byte_buffer[idx] == GPIO_REQUEST_WRITE){
                // request write gpio
                printk("GPIO_WRITE\n");
            }
            if(*recv_byte_buffer[idx] == GPIO_REQUEST_READ){
                //request read gpio
                printk("GPIO_READ\n");
            }
            else if(*recv_byte_buffer[idx] == END_SIGNAL){
                *send_byte_buffer[idx] = 1;
                ret = kernel_sendmsg(connection_sock[idx], send_msg[idx], send_vec[idx], 1, 1);
                if(ret < 0){
                    printk("rpc_raspberry_receiver: send error!!\n");
                }
                else{
                    printk("rpc_raspberry_receiver: send ok!!\n");
                }
                break;
            }
        }
    }
    return ret;
}



static int wait_connection(void){
    int ret;
    //need to implement synchronization,multiplexing IO
    if(socketIdx < MAX_CONNECTION){
        // sock = (struct socket *)kmalloc(sizeof(struct socket),GFP_KERNEL);  
        ret=kernel_accept(sock, &connection_sock[socketIdx], 10);
        if(ret<0){  
            printk("rpc_raspberry_receiver: accept error!\n");  
            ret = -1;
        }
        else{
            printk("rpc_raspberry_receiver: accept ok!\n");
            ret = 1;
            main_task[socketIdx] = kthread_run(mainHandler, (void *)socketIdx, "maintask");   
            socketIdx++;
        }
    }
    return ret;
}

static int rpc_raspberry_receiver_open(struct inode* inode, struct file* file){
    printk("rpc_raspberry_receiver: open\n");
    return 0;
}

static int rpc_raspberry_receiver_release(struct inode* inode, struct file* file){
    printk("rpc_raspberry_receiver: release\n");
    return 0;
}

static long rpc_raspberry_receiver_ioctl(struct file* file, unsigned int cmd, unsigned long arg){
    int ret = 0;
    unsigned long* port_arg;
    printk("rpc_raspberry_receiver: IOCTL\n");
    switch(cmd){
        case INIT:
            port_arg = (unsigned long*)arg;
            init_socket(port_arg);
            break;
        case WAIT_CONNECTION:
            wait_connection();
            break;
    }
    return ret;
}


struct file_operations rpc_raspberry_receiver_ops = {
    .open = rpc_raspberry_receiver_open,
    .release = rpc_raspberry_receiver_release,
    .unlocked_ioctl = rpc_raspberry_receiver_ioctl
};

static int __init rpc_raspberry_receiver_init(void){
    int i = 0;
    printk("rpc_raspberry_receiver: Init Module\n");
    alloc_chrdev_region(&dev_num, 0, 1, DEV_NAME);
    cd_cdev = cdev_alloc();
    cdev_init(cd_cdev, &rpc_raspberry_receiver_ops);
    cdev_add(cd_cdev, dev_num, 1);

    for(i = 0; i < MAX_CONNECTION; i++){
        send_vec[i] = kmalloc(sizeof(struct kvec), GFP_KERNEL);
        recv_vec[i] = kmalloc(sizeof(struct kvec), GFP_KERNEL);
        send_msg[i] = kmalloc(sizeof(struct msghdr), GFP_KERNEL);
        recv_msg[i] = kmalloc(sizeof(struct msghdr), GFP_KERNEL);
        send_byte_buffer[i] = kmalloc(sizeof(char), GFP_KERNEL);
        recv_byte_buffer[i] = kmalloc(sizeof(char), GFP_KERNEL);
        memset(send_vec[i], 0, sizeof(struct kvec));
        memset(recv_vec[i], 0, sizeof(struct kvec));
        memset(send_msg[i], 0, sizeof(struct msghdr));
        memset(recv_msg[i], 0, sizeof(struct msghdr));
        memset(send_byte_buffer[i], 0, sizeof(char));
        memset(recv_byte_buffer[i], 0, sizeof(char));
        recv_msg[i]->msg_flags = MSG_NOSIGNAL;    
    }


    return 0;
}

static void __exit rpc_raspberry_receiver_exit(void){
    int i;
    printk("rpc_raspberry_receiver: Exit Module\n");

    if(sock != NULL){
        sock_release(sock);
    }
    for(i = 0; i < MAX_CONNECTION; i++){
        if(connection_sock[i] != NULL)
            sock_release(connection_sock[i]);
    }

    cdev_del(cd_cdev);
    unregister_chrdev_region(dev_num, 1);
    for(i = 0; i < MAX_CONNECTION; i++){
        kfree(send_vec[i]);
        kfree(recv_vec[i]);
        kfree(send_msg[i]);
        kfree(recv_msg[i]);
        kfree(send_byte_buffer[i]);
        kfree(recv_byte_buffer[i]);
    }
}

module_init(rpc_raspberry_receiver_init);
module_exit(rpc_raspberry_receiver_exit);






