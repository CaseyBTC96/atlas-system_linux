#include "_strace.h"

/**
 * main - Entry point to our function
 * @argc: number of arguments
 * @argv: the argument vector
 *
 * Return: 0 or 1
 */

int main(int argc, char **argv, char **env)
{
	if (argc < 2)
	{
		perror("not enough args!");
		return (1);
	}

	pid_t child;

	child = fork();
	if (child == 0)
	{
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execve(argv[1], &argv[1], env);
	}
	else
	{
		parent_process(child);
	}
	return (0);
}


/**
 * parent_process - handles the parent process
 * @child: the child PID
 *
 * Return: 0 or 1
 */

int parent_process(pid_t child)
{
	int status, entry = 0;
	struct user_regs_struct regs;

	while (1)
	{
		if(waitpid(child, &status, 0) == -1)
			perror("waitPID");

		if (WIFEXITED(status))
			break;

		ptrace(PTRACE_GETREGS, child, NULL, &regs);

		if (entry == 0 || entry % 2 != 0)
		{
			printf("%llu\n", regs.orig_rax);
		}
		entry++;

		if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1)
			return (1);
		fflush(NULL);
	}
	return (0);
}
