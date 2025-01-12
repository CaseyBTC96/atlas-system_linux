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
		if (waitpid(child, &status, 0) == -1)
			return (1);

		if (WIFEXITED(status) || WIFSIGNALED(status))
			break;

		if (WIFSTOPPED(status))
		{
			syscall_t const *callinfo;

			if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
				perror("ptrace_regs"), exit(1);

			if (entry == 0 || entry % 2 != 0)
			{
				callinfo = &syscalls_64_g[regs.orig_rax];
				printf("%s\n", callinfo->name);
			}
			entry++;
		}

		if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1)
			return (1);
		fflush(NULL);
	}
	return (0);
}
