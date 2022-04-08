#pragma once

#include <linux/semaphore.h>
#include <linux/types.h>
#include <linux/dirent.h>
#include <linux/compat.h>
#include <linux/stat.h>
#include <linux/syscalls.h>

#include "error_code.h"

/**
 * @brief  Hook a syscall and return a pointer to the original syscall handler
 */
void *hook_syscall(unsigned long *sys_call_table, uint16_t syscall_index, void *hook_addr);

error_code_e SYSCALL_HOOKS_init();
void SYSCALL_HOOKS_exit();

extern unsigned long *sys_call_table;
