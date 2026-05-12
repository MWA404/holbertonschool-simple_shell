#include "shell.h"

/**
 * execute_command - executes a command
 * @line: command line
 */
void execute_command(char *line)
{
	pid_t pid;
	char *argv[2];

	argv[0] = line;
	argv[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return;
	}

	if (pid == 0)
	{
		if (execve(argv[0], argv, environ) == -1)
		{
			perror(argv[0]);
			exit(1);
		}
	}

	wait(NULL);
}

/**
 * main - simple UNIX command line interpreter
 *
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 2);

		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			free(line);
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			return (0);
		}

		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		execute_command(line);
	}
}
