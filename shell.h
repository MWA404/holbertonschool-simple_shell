#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;
void execute_command(char *line);
char *clean_line(char *line, ssize_t read);
#endif

