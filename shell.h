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
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct env_s
{
	char *name;
	char *value;
	struct env_s *next;
} env_t;

char **buildEnv(env_t *head, int *envsize);
env_t *addEnv(env_t **head, char *name, char *value);
void free_list(env_t *head);
env_t *initEnvList(char **environ, env_t **head);
int checkPath(char *inp, char *argv[], char *save, env_t *head);
int _putchar(char c);
int unsetEnv(char *name, env_t **head);
void _putstring(char *str);
int runProg(char *name, char *argv[], env_t *head);
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
int checkBuiltins(char *tok, char *save, env_t **head);
int _strcmp(char *s1, char *s2);
env_t *getEnvPtr(char *name, env_t *head);
char **getArgs(char *tok, char *argv[], char *save);
void freeArgs(char **args, int envsize);
int cdBuiltin(char *save, env_t *head);
env_t *setEnvPtr(char *envname, char *value, env_t *head);
char *str_concat(char *s1, char *s2);
int _atoi(char *s);
#endif
