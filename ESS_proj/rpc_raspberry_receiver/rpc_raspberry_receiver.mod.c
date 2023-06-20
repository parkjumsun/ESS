#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xad1a7def, "module_layout" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x1d11aa3a, "cdev_del" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0x42160169, "flush_workqueue" },
	{ 0x37a0cba, "kfree" },
	{ 0x43a53735, "__alloc_workqueue_key" },
	{ 0x8899dc97, "kmem_cache_alloc_trace" },
	{ 0x88647e86, "cdev_add" },
	{ 0x7a3acc89, "cdev_init" },
	{ 0x61a76852, "kmalloc_caches" },
	{ 0xb4673642, "cdev_alloc" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x3520ac6, "sock_release" },
	{ 0xc1514a3b, "free_irq" },
	{ 0xd6b8e852, "request_threaded_irq" },
	{ 0x72c365ef, "gpiod_to_irq" },
	{ 0x1b6314fd, "in_aton" },
	{ 0x16e02b99, "gpiod_get_raw_value" },
	{ 0xffb0dcd9, "gpiod_set_raw_value" },
	{ 0x350a004, "gpio_to_desc" },
	{ 0xfe990052, "gpio_free" },
	{ 0x403f9529, "gpio_request_one" },
	{ 0xe9d3623c, "kernel_recvmsg" },
	{ 0xa58fca6a, "kernel_sendmsg" },
	{ 0x9d669763, "memcpy" },
	{ 0xdb7305a1, "__stack_chk_fail" },
	{ 0x9b21e916, "wake_up_process" },
	{ 0x62eba9, "kthread_create_on_node" },
	{ 0xdb9ca3c5, "_raw_spin_lock" },
	{ 0xeda1e9d0, "kernel_accept" },
	{ 0x14c0eddc, "kernel_listen" },
	{ 0x3df276d2, "kernel_bind" },
	{ 0xb68e1513, "sock_create_kern" },
	{ 0xe1cc8c68, "init_net" },
	{ 0x5f754e5a, "memset" },
	{ 0x28cc25db, "arm_copy_from_user" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xb2d48a2e, "queue_work_on" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x7c32d0f0, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "A87CF7FB1D4DC42B52F90C1");
