#include "shell.h"


/**
 * get_path - gets PATH value from environ
 * Return: PATH value or NULL
 */
char *get_path(void)
{
	int i;

	for (i = 0; environ[i] != NULL; i++)
	{
		if (environ[i][0] == 'P' && environ[i][1] == 'A' &&
			environ[i][2] == 'T' && environ[i][3] == 'H' &&
			environ[i][4] == '=')
			return (environ[i] + 5);
	}
	return (NULL);
}

/**
 * find_path - finds the full path of a command
 * @cmd: the command to find
 * Return: full path or NULL if not found
 */
char *find_path(char *cmd)
{
	char *path, *path_copy, *dir, *full_path;
	struct stat st;

	/* if cmd starts with . or / use it directly */
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (stat(cmd, &st) == 0)
			return (cmd);
		return (NULL);
	}

	/* get PATH from environment without getenv */
	path = get_path();
	if (path == NULL || path[0] == '\0')
		return (NULL);

	/* make a copy because strtok modifies the string */
	path_copy = strdup(path);
	if (path_copy == NULL)
		return (NULL);

	/* search each directory in PATH */
	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		/* build full path: dir + "/" + cmd */
		full_path = build_path(dir, cmd);
		if (full_path == NULL)
		{
			free(path_copy);
			return (NULL);
		}
		if (stat(full_path, &st) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

/**
 * execute_command - executes a command
 * @line: command line
 * @argv0: name of the shell
 */
void execute_command(char *line, char *argv0)
{
	pid_t pid;
	char *argv[10];
	char *token;
	char *full_path;
	int i;

	/* split command into arguments */
	token = strtok(line, " ");
	i = 0;
	while (token != NULL)
	{
		argv[i] = token;
		token = strtok(NULL, " ");
		i++;
	}
	argv[i] = NULL;

	if (strcmp(argv[0], "exit") == 0)
		exit(0);
	/* find the full path of the command */
	full_path = find_path(argv[0]);
	if (full_path == NULL)
	{
		/* don't fork if command not found */
		fprintf(stderr, "%s: 1: %s: not found\n", argv0, argv[0]);
		return;
	}

	/* fork and execute */
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		if (full_path != argv[0])
			free(full_path);
		return;
	}
	if (pid == 0)
	{
		if (execve(full_path, argv, environ) == -1)
		{
			perror(argv[0]);
			if (full_path != argv[0])
				free(full_path);
			exit(1);
		}
	}
	wait(NULL);
	if (full_path != argv[0])
		free(full_path);
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
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: last exit status
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	char *cmd;
	size_t len = 0;
	ssize_t read;

	(void)argc;
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
			execute_command(cmd, argv[0]);
	}
}
