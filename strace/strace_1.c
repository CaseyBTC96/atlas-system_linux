#include "syscalls.h"
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/user.h>

int tracerLoop(pid_t child_pid)
{
    int status;
    struct user_regs_struct regs;
    int syscall_return = 1;

    while (1)
    {
        if (wait(&status) == -1)
            return (1);
        if (WIFEXITED(status))
            break;

        if (!syscall_return)
        {
            if (ptrace(PTRACE_GETREGS, child_pid, NULL, &regs) == -1)
                return (1);
            if (regs.orig_rax < PT_SYSCALL)
                printf("%s\n", syscalls_64_g[regs.orig_rax].name);
        }

        if (ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) == -1)
            return (1);

        syscall_return = !syscall_return;
    }
    return (0);
}

int main(int argc, char *argv[])
{
    pid_t child;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    child = fork();
    if (child == -1)
        return EXIT_FAILURE;

    if (child == 0)
    {
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1)
            return EXIT_FAILURE;
        execvp(argv[1], &argv[1]);
        return EXIT_FAILURE;
    }

    return tracerLoop(child);
}
