#include "syscall_hooks.h"

asmlinkage long (*original_getdents)(const struct pt_regs *);

asmlinkage long sys_getdents_hook(const struct pt_regs *regs)
{
	// printk("%s %ld dirent=%p count=%ld bytes_read=%d\n", __func__, (long)regs->di, (void *)regs->si, (long)regs->dx, bytes_read);
	return 0;
}