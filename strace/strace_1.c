#include "syscalls.h"

/**
 * main - The entry point of the program
 * @argc: The number of command-line arguments passed to the program.
 * @argv: An array of strings representing the command-line arguments.
 * @envp: An array of strings representing the environment variables.
 * 
 * This function creates a child process, attaches a tracer to it, and executes
 * the specified program within the child process. The parent process monitors
 * the child process, prints the system call numbers made by the child, and 
 * handles termination of the child process.
 * 
 * Return: Returns 0 on successful execution, or exits with status 1 if an error occurs.
 */
int main(int argc, const char *argv[], char *const envp[])
{
    pid_t child;
    int status, syscall_count = 0;
    struct user_regs_struct regs;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return 1; // Return 1 to indicate an error in usage
    }

    child = fork();
    // if (child == -1)
    // {
    //     perror("fork");
    //     return -1;
    // }

    if (child == 0)
    {
        // In the child process
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1)
        {
            perror("ptrace");
            return 1; // Return 1 to indicate an error in ptrace
        }
        execve(argv[1], (char *const *)(argv + 1), (char *const *)envp);
        perror("execve"); // Handle execve error
        return 1; // Return 1 to indicate an error in execve
    }
    else
    {
        while (2)
        {
            ptrace(PT_SYSCALL, child, NULL, NULL);
            wait(&status);
            // if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1)
            // {
            //     perror("ptrace");
            //     // return -1;
            // }

            if (WIFEXITED(status))
            {
                break;
            }
            ptrace(PTRACE_GETREGS, child, NULL, &regs);
            // if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
            // {
            //     perror("ptrace");
            //     // return -1;
            // }

            if (syscall_count == 0 || syscall_count % 2 != 0)
            {
                if (regs.orig_rax != 1)
                {
                    fprintf(stderr, "%s\n", NAMES);
                }
                else
                {
                    fprintf(stderr, "%s", NAMES);
                }
            }

            if (syscall_count % 2 == 0 && regs.orig_rax == 1)
            {
                fprintf(stderr, "\n");
            }

            syscall_count++;
        }
    }

    return 0;
}


// Functions to print specific registers
void print_rdi(struct user_regs_struct *regs)
{
    fprintf(stderr, "%#lx", (size_t)regs->rdi);
}

void print_rsi(struct user_regs_struct *regs)
{
    fprintf(stderr, ", %#lx", (size_t)regs->rsi);
}

void print_rdx(struct user_regs_struct *regs)
{
    fprintf(stderr, ", %#lx", (size_t)regs->rdx);
}

void print_r10(struct user_regs_struct *regs)
{
    fprintf(stderr, ", %#lx", (size_t)regs->r10);
}

void print_r8(struct user_regs_struct *regs)
{
    fprintf(stderr, ", %#lx", (size_t)regs->r8);
}

void print_r9(struct user_regs_struct *regs)
{
    fprintf(stderr, ", %#lx", (size_t)regs->r9);
}

static inline int get_regs(pid_t child, struct user_regs_struct *regs)
{
    return ptrace(PTRACE_GETREGS, child, NULL, regs) == -1 ? -1 : 0;
}

static inline int should_print(int check)
{
    return check == 0 || (check % 2 != 0);
}
