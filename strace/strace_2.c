#include "syscalls.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/syscall.h>

const char *get_syscall_name(long syscall) {
    static char unknown[32];
    switch (syscall) {
        case SYS_execve: return "execve";
        case SYS_brk: return "brk";
        case SYS_access: return "access";
        case SYS_mmap: return "mmap";
        case SYS_open: return "open";
        case SYS_fstat: return "fstat";
        case SYS_close: return "close";
        case SYS_read: return "read";
        case SYS_mprotect: return "mprotect";
        case SYS_arch_prctl: return "arch_prctl";
        case SYS_munmap: return "munmap";
        case SYS_write: return "write";
        case SYS_exit_group: return "exit_group";
        default:
            snprintf(unknown, sizeof(unknown), "syscall_%ld", syscall);
            return unknown;
    }
}

void trace_process(pid_t child_pid) {
    int status, in_syscall = 0;
    struct user_regs_struct regs;

    waitpid(child_pid, &status, 0);
    ptrace(PTRACE_SETOPTIONS, child_pid, 0, PTRACE_O_TRACESYSGOOD);

    while (1) {
        if (ptrace(PTRACE_SYSCALL, child_pid, 0, 0) == -1)
            break;
        if (waitpid(child_pid, &status, 0) == -1)
            break;
        if (WIFEXITED(status))
            break;

        ptrace(PTRACE_GETREGS, child_pid, 0, &regs);

        if (!in_syscall) {
            printf("%s", get_syscall_name(regs.orig_rax));
            in_syscall = 1;
        } else {
            if (regs.orig_rax == SYS_exit_group)
                printf(" = ?\n");
            else if (regs.orig_rax == SYS_write) {
                char *buf = malloc(regs.rdx + 1);
                for (unsigned int i = 0; i < regs.rdx; i++) {
                    long val = ptrace(PTRACE_PEEKDATA, child_pid, regs.rsi + i, NULL);
                    buf[i] = val & 0xFF;
                }
                buf[regs.rdx] = '\0';
                printf("%s = %llx\n", buf, regs.rax);
                free(buf);
            } else
                printf(" = %llx\n", regs.rax);
            in_syscall = 0;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s command [args...]\n", argv[0]);
        return 1;
    }

    pid_t child_pid = fork();
    if (child_pid == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execv(argv[1], &argv[1]);
        exit(1);
    }

    trace_process(child_pid);
    return 0;
}
