#ifndef SYSCALLS
#define SYSCALLS

#include <linux/semaphore.h>
#include <linux/types.h>
#include <linux/dirent.h>
#include <linux/compat.h>
#include <linux/stat.h>
#include <linux/syscalls.h>

struct linux_dirent {
	unsigned long d_ino;
	unsigned long d_off;
	unsigned short d_reclen;
	char d_name[1];
};


// Functions
asmlinkage long sys_getdents_hook(const struct pt_regs * regs);
extern asmlinkage long (*original_getdents) (const struct pt_regs * regs);

#endif
