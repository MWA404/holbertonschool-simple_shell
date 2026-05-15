#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>

extern char **environ;
char *clean_line(char *line, ssize_t read);
void execute_command(char *line, char *argv0);
char *find_path(char *cmd);
#endif

