#include "syscall_hooks.h"
#include "mem_protection.h"

unsigned long *sys_call_table = NULL;

asmlinkage long sys_sendmsg_hook(const struct pt_regs *regs);
asmlinkage long (*original_sendmsg)(const struct pt_regs *);

error_code_e SYSCALL_HOOKS_init()
{
	sys_call_table = (unsigned long *)kallsyms_lookup_name("sys_call_table");
	if (sys_call_table == NULL)
	{
		printk(KERN_ERR "Could not lookup 'sys_call_table'\n");
		return CODE_KALLSYMS_FAILED;
	}
	printk(KERN_INFO "The address of sys_call_table is: %p\n", sys_call_table);

	// Hook stuff
	original_sendmsg = hook_syscall(sys_call_table, __NR_sendmsg, sys_sendmsg_hook);
	printk(KERN_INFO "%s: Finished\n", __func__);

	return CODE_SUCCESS;
}

void SYSCALL_HOOKS_exit()
{
	// Restore hooks to original functions
	(void)hook_syscall(sys_call_table, __NR_sendmsg, original_sendmsg);
	printk(KERN_INFO "%s: Finished\n", __func__);
}

void *hook_syscall(unsigned long *sys_call_table, uint16_t syscall_index, void *hook_addr)
{
	void *old_syscall = (void *)sys_call_table[syscall_index];

	disable_write_protection();
	sys_call_table[syscall_index] = (unsigned long)hook_addr;
	enable_write_protection();

	return old_syscall;
}

asmlinkage long sys_sendmsg_hook(const struct pt_regs *regs)
{
	int fd = (int)regs->di;
	struct user_msghdr __user *msg = (struct user_msghdr *)regs->si;
	unsigned int flags = (unsigned int)regs->dx;

	long original_sendmsg_result = original_sendmsg(regs);
	printk("%s: fd=%d msg=%p flags=%u\n", __func__, fd, msg, flags);
	return original_sendmsg_result;
}