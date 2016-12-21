#ifndef SHELL_H
#define SHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

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
#define HIST_FILE ".simple_shell_history"

/**
 * struct env_s - a singly linked list
 * @name: the environment variable
 * @value: the environment variable value
 * @next: the pointer to the next node of struct env_s
 *
 * Description: the struct that stores the environment variables
 */
typedef struct env_s
{
	char *name;
	char *value;
	struct env_s *next;
} env_t;

/**
 * struct hist_s - a singly linked list
 * @entry: pointer to the char* of a user's session
 * @next: pointer to the next linked list
 *
 * Description: the struct of the user's history
 */
typedef struct hist_s
{
	char *entry;
	struct hist_s *next;
} hist_t;
/**
 * struct helper_s - a helper struct
 * @hist_head: head to the hist linked list
 * @env: head to the env hist linke list
 * @total:
 * @bufsize:
 * @printed:
 * @last:
 * @pid:
 * @lastExit;
 * @file:
 * @type:
 *
 * Description:
 */
typedef struct helper_s
{
	char *bufhead;
	hist_t *hist_head;
	env_t *env;
	long *total;
	int *bufsize;
	int *printed;
	int *last;
	char *pid;
	int lastExit;
	int file;
	int type;
} helper_t;


char *moreLines(helper_t *helper, char *buf);
char *parseDelimiters(char *buf, helper_t *helper);
void countBuf(char *buf, helper_t *helper);
void countLine(char *buf, helper_t *helper);

/* builtins */
void exitBuiltin(char *tok, char *inp, helper_t *helper);
int listEnv(env_t **environ);
int cdBuiltin(char *save, helper_t *helper);
int checkBuiltins(char *tok, char *save, helper_t *helper);
int unsetEnv(char *name, env_t **head);
env_t *setEnvPtr(char *envname, char *value, env_t *head);

/* mem.c prototypes */
char *_memcpy(char *dest, char *src, unsigned int n);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);

/* getline.c prototype */
char *get_line(int file, helper_t *helper);
char *parseDollar(char *buf, helper_t *helper);
char *parseWhitespace(char *buf);
char *innerCat(char *buf, char *string, int *bufsize, int insert);
char *sliceString(char *buf, int *bufsize, int slicesize, int index);
char *expandBuffer(char *buf, int bufsize, int newsize);

int getTermType(int file);

/* envlist.c prototypes */
env_t *initEnvList(char **environ, env_t **head);
char **buildEnv(env_t *head, int *envsize);
void free_list(env_t *head);
env_t *addEnv(env_t **head, char *name, char *value);
env_t *getEnvPtr(char *name, env_t *head);


/* main.c prototypes */
helper_t *initHelper(env_t *env, hist_t *hist_head, char *pid);
int checkPath(char *inp, char *argv[], char *save, helper_t *helper);
void freeArgs(char **args, int envsize);
char **getArgs(char *tok, char *argv[], char *save);
int runProg(char *name, char *argv[], helper_t *helper);

char *_getpid(void);

/* stringlib.c prototypes */
int _strlen(char *s);
char *_strcpy(char *dest, char *src);
char *_strdup(char *str);
char *str_concat(char *s1, char *s2);
int _strcmp(char *s1, char *s2);

/* specialstrings.c prototypes */
char *splitstr(char *str, const char *delim, char **saveptr);
int allstrcmp(char *s1, char *s2);
char *dir_concat(char *s1, char *s2);
int _atoi(char *s);

/* io.c prototypes */
void prompt(void);
void _putstring(char *str);
int _putchar(char c);
void _putint(int i, int flag);

/* math.c prototypes */
int itoa(int n, char *s);
int _abs(int num);

/* history.c prototypes*/
hist_t *add_hist(int total, hist_t **hist_head, char *buf);
void clear_hist(hist_t **hist_head);
void print_hist(hist_t *hist_head);
void push_hist(hist_t *hist_head, env_t *head);
hist_t *pull_hist(hist_t **hist_head, env_t *head);

/* helpBuildIn prototypes */
int helpBuiltIn(char *file);

char *whitespace(char *buf, helper_t *helper);

/* help_1.c prototypes */
void helpExit(void);
void helpEnv(void);
void helpSetEnv(void);
void helpUnsetEnv(void);
void helpCD(void);

/* help_2.c prototypes */
void helpHelp(void);
void helpAlias(void);
void helpHistory(void);


void sighandler(int signum);

helper_t *setupMain(int argc, char **argv, char **envp);
int checkLocal(char *tok, helper_t *helper, char *save);
char *bufferDelete(char *buf, helper_t *helper, int index, int times);
char *parseComments(char *buf, helper_t *helper);
int isDelimiter(char c);
int isWhitespace(char c);
#endif
