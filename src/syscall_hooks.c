#include "syscall_hooks.h"
#include "mem_protection.h"

unsigned long *sys_call_table = NULL;

asmlinkage long sys_sendto_hook(const struct pt_regs *regs);
asmlinkage long (*original_sendto)(const struct pt_regs *);

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
	original_sendto = hook_syscall(sys_call_table, __NR_sendto, sys_sendto_hook);
	printk(KERN_INFO "%s: Finished\n", __func__);

	return CODE_SUCCESS;
}

void SYSCALL_HOOKS_exit()
{
	(void)hook_syscall(sys_call_table, __NR_sendto, original_sendto);
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

asmlinkage long sys_sendto_hook(const struct pt_regs *regs)
{
	int fd = regs->di;
	void __user *buff = (void *)regs->si;
	size_t len = regs->dx;
	unsigned int flags = regs->r10;
	struct sockaddr __user *addr = (struct sockaddr *)regs->r8;
	int addr_len = regs->r9;

	long original_sendto_result = original_sendto(regs);
	printk("%s: fd=%d buff=%p len=%zu flags=%u addr=%p addr_len=%d\n", __func__, fd, buff, len, flags, addr, addr_len);
	return original_sendto_result;
}