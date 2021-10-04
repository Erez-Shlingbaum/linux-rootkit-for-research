#include "syscalls.h"
#include "list.h"


asmlinkage long (*original_getdents) (const struct pt_regs *);
asmlinkage long sys_getdents_hook(const struct pt_regs * regs)
{
	int rtn;
	int i = 0;

	struct linux_dirent *cur = (struct linux_dirent *)regs->si;
	rtn = original_getdents(regs);
	printk (KERN_INFO "after orig get dents \n");
	return rtn;
}
