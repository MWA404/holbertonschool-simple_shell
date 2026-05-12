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
 * clean_line - removes new line and spaces
 * @line: command line
 * @read: number of characters read
 *
 * Return: clean command
 */
char *clean_line(char *line, ssize_t read)
{
	char *cmd;

	if (line[read - 1] == '\n')
		line[read - 1] = '\0';

	cmd = line;
	while (*cmd == ' ')
		cmd++;

	read = 0;
	while (cmd[read] != '\0')
		read++;

	while (read > 0 && cmd[read - 1] == ' ')
	{
		cmd[read - 1] = '\0';
		read--;
	}

	return (cmd);
}

/**
 * main - simple UNIX command line interpreter
 *
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL;
	char *cmd;
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

		cmd = clean_line(line, read);
		if (*cmd != '\0')
			execute_command(cmd);
	}
}
