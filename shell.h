#ifndef SHELL_H
#define SHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
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
/**
 * struct env_s - a singly linked list
 * @name:
 * @value:
 * @next:
 *
 * Description:
 */
typedef struct env_s
{
	char *name;
	char *value;
	struct env_s *next;
} env_t;
/**
 * struct hist_s - a singly linked list
 * @name:
 * @value:
 * @next:
 *
 * Description:
 */
typedef struct hist_s
{
	char *entry;
	struct hist_s *next;
} hist_t;

/* mem.c prototypes */
char *_memcpy(char *dest, char *src, unsigned int n);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
/* getline.c prototype */
char *get_line(int file, hist_t **hist_head);
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
void prompt(void);
int _strlen(char *s);
char *_strcpy(char *dest, char *src);
char *dir_concat(char *s1, char *s2);
void addHistory(char **history, char *command);
char *trim(char *string, int count);
char *_strdup(char *str);
int checkBuiltins(char *inp, char *save, env_t **environ, hist_t **hist_head);
int _strcmp(char *s1, char *s2);
env_t *getEnvPtr(char *name, env_t *head);
char **getArgs(char *tok, char *argv[], char *save);
void freeArgs(char **args, int envsize);
int cdBuiltin(char *save, env_t *head);
env_t *setEnvPtr(char *envname, char *value, env_t *head);
char *str_concat(char *s1, char *s2);
int _atoi(char *s);
/* history.c prototypes*/
hist_t *add_hist(int total, hist_t **hist_head, char *buf);
void clear_hist(hist_t **hist_head);
void print_hist(hist_t *hist_head);
#endif
