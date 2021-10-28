#include "syscalls.h"
#include "list.h"

asmlinkage long sys_getdents_hook(const struct pt_regs * regs);
asmlinkage long sys_getdents32_hook(const struct pt_regs * regs);
asmlinkage long (*original_getdents) (const struct pt_regs *);
asmlinkage long (*original_getdents_32)(const struct pt_regs *);

/*
                if (arch == AUDIT_ARCH_X86_64) {
                        sd.args[0] = regs->di;
                        sd.args[1] = regs->si;
                        sd.args[2] = regs->dx;
                        sd.args[3] = regs->r10;
                        sd.args[4] = regs->r8;
                        sd.args[5] = regs->r9;
                } else
#endif
                {
                        sd.args[0] = regs->bx;
                        sd.args[1] = regs->cx;
                        sd.args[2] = regs->dx;
                        sd.args[3] = regs->si;
                        sd.args[4] = regs->di;
                        sd.args[5] = regs->bp;
                }

 */


struct compat_linux_dirent {
        compat_ulong_t  d_ino;
        compat_ulong_t  d_off;
        unsigned short  d_reclen;
        char            d_name[1];
};


asmlinkage long sys_getdents_hook(const struct pt_regs * regs)
{
	int bytes_read;
	int fd = regs->di;
	int buf_size =  regs->dx;
	struct compat_linux_dirent *dirent = (struct compat_linux_dirent *)regs->si;
	struct compat_linux_dirent* lastdirent = 0;

	bytes_read = original_getdents(regs);
	printk("%s %ld dirent=%p count=%ld bytes_read=%d\n",__func__ ,(long)regs->di, 
			(void *)regs->si, 
			(long)regs->dx, 
			bytes_read);
	
	lastdirent = (struct compat_linux_dirent *) ((void *)dirent + bytes_read);

        while (dirent < lastdirent){
                printk("| %s\n", dirent->d_name);
                dirent = (struct compat_linux_dirent *)((unsigned char*)dirent + dirent->d_reclen);
        }


	printk (KERN_INFO "after orig getdents64 \n");
	return bytes_read;
}
asmlinkage long sys_getdents32_hook(const struct pt_regs * regs)
{
	int bytes_read;
	int fd = regs->di;
	struct linux_dirent *temp = (struct linux_dirent *)regs->si;
	int count = regs->dx;
	struct linux_dirent *lastdirent;

	bytes_read = original_getdents_32(regs);
	//printk("%s %ld %p cnt=%d bytes=%d\n",__func__ ,fd, 
	//		(void *)regs->si, count, bytes_read);

	lastdirent = (struct linux_dirent *) ((long)regs->si + bytes_read);
 
        while (temp < lastdirent){
                printk("%s | ", temp->d_name);
                temp = (struct linux_dirent *)((unsigned char*)temp + temp->d_reclen);
        }


	printk (KERN_INFO "\n");
	return bytes_read;
}

