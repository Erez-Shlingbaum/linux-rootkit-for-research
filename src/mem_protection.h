#ifndef MEM_PROTECTION_H
#define MEM_PROTECTION_H

#include <asm/cacheflush.h>
#include <linux/kallsyms.h>

#define disable_write_protection()      \
    do                                  \
    {                                   \
        unsigned long cr0 = read_cr0(); \
        write_cr0(cr0 &(~0x10000));     \
    } while (false)

#define enable_write_protection()       \
    do                                  \
    {                                   \
        unsigned long cr0 = read_cr0(); \
        write_cr0(cr0 | 0x10000);       \
    } while (false)

#endif
