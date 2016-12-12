#ifndef SHELL_H
#define SHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
extern char **environ;
#define BUFSIZE 1024
#define ARGS 256
#define PATHSIZE 10
int checkPath(char *inp, char *argv[], char *save);
int _putchar(char c);
void _putstring(char *str);
int runProg(char *name, char *argv[]);
char *splitstr(char *str, const char *delim, char **saveptr);
int allstrcmp(char *s1, char *s2);
char *get_line(const int file);
void prompt(void);
int _strlen(char *s);
char *_strcpy(char *dest, char *src);
char *dir_concat(char *s1, char *s2);
void addHistory(char **history, char *command);
char *trim(char *string, int count);
char *_strdup(char *str);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
int checkBuiltins(char *tok, char *save);
int _strcmp(char *s1, char *s2);
char *getEnvPtr(char *name);
char **getArgs(char *tok, char *argv[], char *save);
void freeArgs(char **args);
int cdBuiltin(char *save);
char *setEnvPtr(char *envname, char *value);
char *str_concat(char *s1, char *s2);
int _atoi(char *s);
#endif
