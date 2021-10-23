#include <asm/cacheflush.h>
#include <linux/kallsyms.h>

struct {
	unsigned short limit;
	unsigned long base;
} __attribute__ ((packed))idtr;

struct {
	unsigned short off1;
	unsigned short sel;
	unsigned char none, flags;
	unsigned short off2;
} __attribute__ ((packed))idt;

void *memmem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen)
{
	char *p;

	for(p = (char *)haystack; p <= ((char *)haystack - needlelen + haystacklen); p++)
		if(memcmp(p, needle, needlelen) == 0)
			return (void *)p;
	return NULL;
}
/*
unsigned long **find_sys_call_table(void)
{
	unsigned long **p;

	unsigned long sct_off = 0;
	unsigned char code[255];
	asm("sidt %0":"=m" (idtr));
	memcpy(&idt, (void *)(idtr.base + 8 * 0x80), sizeof(idt));
	sct_off = (idt.off2 << 16) | idt.off1;
	memcpy(code, (void *)sct_off, sizeof(code));

	p = (char **)memmem(code, sizeof(code), "\xff\x14\x85", 3);

	p=kallsyms_lookup_name("sys_call_table");
	if(p)
	{
		printk (KERN_INFO "fouund sys call table\n");
		return p;
//xxyy		return *(unsigned long **)((char *)p + 3);
	}
	else
		return NULL;
}
*/

void disable_write_protection(void)
{
	unsigned long value;

	asm volatile("mov %%cr0,%0" : "=r" (value));
	if (value & 0x00010000) {
		value &= ~0x00010000;
		asm volatile("mov %0,%%cr0": : "r" (value));
	}
}

void enable_write_protection(void)
{
	unsigned long value;
	asm volatile("mov %%cr0,%0" : "=r" (value));
	if (!(value & 0x00010000)) {
		value |= 0x00010000;
		asm volatile("mov %0,%%cr0": : "r" (value));
	}
}
