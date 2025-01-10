#include "syscalls.h"

/**
 * main - entrance to program
 * @argc: Number of arguments
 * @argv: array of arguments
 * @envp: Environ variable
 * Return: returns -1 on fail, else 0
*/

int main(int argc, const char *argv[], char *const envp[])
{
	pid_t child;
	int status, print_check = 0;
	struct user_regs_struct regs;

	if (argc < 2)
	{
		fprintf(stderr, "Unsupported number of Arguments\n");
		return (-1);
	}
	child = fork();
	if (child == 0)
	{
		ptrace(PTRACE_TRACEME, child, NULL, NULL);
		execve(argv[1], (char * const *)(argv + 1), (char * const *)envp);
	}
	else
	{
		while (1)
		{
			ptrace(PT_SYSCALL, child, NULL, NULL);
			wait(&status);
			ptrace(PTRACE_GETREGS, child, NULL, &regs);
			if (WIFEXITED(status))
			{
				fprintf(stderr, " = ?\n");
				break;
			}
			if (print_check == 0 || print_check % 2 != 0)
				fprintf(stderr, "%s", SYSNAME);
			if (print_check % 2 == 0)
			{
				if (regs.orig_rax != 1)
					fprintf(stderr, " = %#lx\n", (size_t)regs.rax);
				else
					fprintf(stderr, " = %#lx\n", (size_t)regs.rax);
			}
			print_check++;
		}
	}
	return (0);
}
