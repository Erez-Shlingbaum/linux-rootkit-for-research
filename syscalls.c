#include "syscalls.h"
#include "list.h"

asmlinkage long sys_getdents_hook(const struct pt_regs * regs);
asmlinkage long sys_getdents32_hook(const struct pt_regs * regs);
asmlinkage long (*original_getdents) (const struct pt_regs *);
asmlinkage long (*original_getdents_32)(const struct pt_regs *);
/*
struct linux_dirent64 {
        u64             d_ino;
        s64             d_off;
        unsigned short  d_reclen;
        unsigned char   d_type;
        char            d_name[0];
};
*/
struct compat_linux_dirent {
        compat_ulong_t  d_ino;
        compat_ulong_t  d_off;
        unsigned short  d_reclen;
        char            d_name[1];
};


asmlinkage long sys_getdents_hook(const struct pt_regs * regs)
{
	int rtn;
//	struct compat_linux_dirent *temp = dirent;
//	struct linux_dirent *cur = (struct linux_dirent *)regs->si;
	printk("%s %ld %p \n",__func__ ,(long)regs->di, (void *)regs->si);
	rtn = original_getdents(regs);
	
/*
        while (temp < lastdirent){
                printk("|||| %s\n", temp->d_name);
                temp = (struct compat_linux_dirent *)((unsigned char*)temp + temp->d_reclen);
        }
*/

	printk (KERN_INFO "after orig getdents \n");
	return rtn;
}
/*
 *
 *#ifdef CONFIG_X86_64
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

asmlinkage long sys_getdents32_hook(const struct pt_regs * regs)
{
	int rtn;
	int fd = regs->di;
	struct linux_dirent *dirent = (struct linux_dirent *)regs->si;
	struct linux_dirent *lastdirent = (struct linux_dirent *)regs->si;

	printk("%s %ld %p \n",__func__ ,(long)regs->di, (void *)regs->si);
	rtn = original_getdents_32(regs);
 /*
        while (temp < lastdirent){
                printk("%s\n", temp->d_name);
                temp = (struct linux_dirent64 *)((unsigned char*)temp + temp->d_reclen);
        }

*/
	printk (KERN_INFO "after orig getdents32 \n");
	return rtn;
}

