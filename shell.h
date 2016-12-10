#ifndef SHELL_H
#define SHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
int _putchar(char c);
void _putstring(char *str);
int runProg(char *name, char *argv[], char *envp[]);
char *splitstr(char *str, const char *delim, char **saveptr);
int allstrcmp(char *s1, char *s2);
char *get_line(const int file);
void prompt();
#endif
