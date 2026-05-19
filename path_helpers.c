#include "shell.h"

/**
 * build_path - builds full path
 * @dir: directory
 * @cmd: command
 *
 * Return: full path or NULL
 */
char *build_path(char *dir, char *cmd)
{
	char *full_path;
	int len;

	len = strlen(dir) + strlen(cmd) + 2;
	full_path = malloc(len);
	if (full_path == NULL)
		return (NULL);

	sprintf(full_path, "%s/%s", dir, cmd);
	return (full_path);
}
