#include "_strace.h"

/**
 * main - Entry point to our function
 * @argc: number of arguments
 * @argv: the argument vector
 *
 * Return: 0 or 1
 */

int main(int argc, char **argv)
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
		child_process(argv[1], &argv[1]);
	}
	else
	{
		parent_process(child);
	}
	return (0);
}

/**
 * child_process - handles the child process
 * @path: the PATH
 * @child_args: the argument vector
 *
 * Return: 0 or 1
 */

int child_process(char *path, char **child_args)
{
	char *env[] = {NULL};

	if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1)
	{
		perror("ptrace child");
	}
	if (execve(path, child_args, env) == -1)
	{
		perror("execve");
	}
	return (1);
}

/**
 * parent_process - handles the parent process
 * @child: the child PID
 *
 * Return: 0 or 1
 */

int parent_process(pid_t child)
{
	int status, entry;

	while (1)
	{
		if (waitpid(child, &status, 0) == -1)
			return (1);

		if (WIFEXITED(status) || WIFSIGNALED(status))
			break;

		if (WIFSTOPPED(status))
		{
			struct user_regs_struct regs;
			syscall_t const *callinfo;

			if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
				perror("ptrace_regs"), exit(1);

			if (entry == 0)
			{
				callinfo = &syscalls_64_g[regs.orig_rax];
				printf("%s\n", callinfo->name);
				entry = 1;
			}
			else
				entry = 0;
		}

		if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1)
			return (1);
	}
	return (0);
}
