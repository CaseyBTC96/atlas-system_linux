#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdlib.h>
#include "syscalls.h"

int child_process(char *, char **);
int parent_process(pid_t);
void print_params(struct user_regs_struct *, syscall_t const *);

#endif
