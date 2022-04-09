#ifndef MEM_PROTECTION_H
#define MEM_PROTECTION_H

#include <asm/cacheflush.h>
#include <linux/kallsyms.h>

extern unsigned long __force_order;
static inline void write_forced_cr0(unsigned long value)
{
    asm volatile("mov %0,%%cr0"
                 : "+r"(value), "+m"(__force_order));
}

#define disable_write_protection()         \
    do                                     \
    {                                      \
        unsigned long cr0 = read_cr0();    \
        write_forced_cr0(cr0 &(~0x10000)); \
    } while (false)

#define enable_write_protection()        \
    do                                   \
    {                                    \
        unsigned long cr0 = read_cr0();  \
        write_forced_cr0(cr0 | 0x10000); \
    } while (false)

#endif
