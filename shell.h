#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

extern char **environ;
char *get_path(void);
char *find_path(char *cmd);
void execute_command(char *line, char *argv0);
char *clean_line(char *line, ssize_t read);

#endif
