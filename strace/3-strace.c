#include "syscalls.h"

void print_params(size_t i, struct user_regs_struct *regs);

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
	size_t i =0;
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
				fprintf(stderr, ") = ?\n");
				break;
			}
			if (print_check == 0 || print_check % 2 != 0)
			{
				fprintf(stderr, "%s(", SYSNAME);
				for (i = 0; i < SYSPARAM; i++)
				{
					if (SYSTYPE == VOID)
						continue;
					if (SYSTYPE == VARARGS)
						fprintf(stderr, ", ...");
					else
						print_params(i, &regs);
				}
			}
			if (print_check % 2 == 0)
				fprintf(stderr, ") = %#lx\n", (size_t)regs.rax);
			print_check++;
		}
	}
	return (0);
}

void print_params(size_t i, struct user_regs_struct *regs)
{
	switch (i)
	{
		case 0:
			fprintf(stderr, "%#lx", (size_t)regs->rdi);
			return;
		case 1:
			fprintf(stderr, ", %#lx", (size_t)regs->rsi);
			return;
		case 2:
			fprintf(stderr, ", %#lx", (size_t)regs->rdx);
			return;
		case 3:
			fprintf(stderr, ", %#lx", (size_t)regs->r10);
			return;
		case 4:
			fprintf(stderr, ", %#lx", (size_t)regs->r8);
			return;
		case 5:
			fprintf(stderr, ", %#lx", (size_t)regs->r9);
			return;
		default:
			return;
	}
}
