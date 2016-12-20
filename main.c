#include "shell.h"

void sighandler(int signum);

/**
 * main - entry point for shell program
 * loops input for a shell, splits them into appropriate actions
 *
 * @argc: argument count
 * @argv: arguments passed
 * Return: return values in man page
 */
int main(int argc, char *argv[], char *env[])
{
	char *save, *tok, *inp, **args;
	char delim = ' ';
	env_t *head;
	helper_t *helper;
	int file, cstatus, type;
	hist_t *hist_head;
	struct stat st;

	hist_head = NULL;
	head = NULL;
	if (argc == 1)
		file = STDIN_FILENO;
	else if (argc == 2)
	{
		file = open(argv[1], O_RDONLY);
		if (file == -1)
		{
			_putstring("Error opening file.");
			_exit(9);
		}
	}
	else
	{
		_putstring("Please run with no argument, or one argument to run from script.");
		_exit(9);
	}
	type = getTermType(file);
	(void) argc; /* need to use this to check to check for scripts later!*/
	signal(SIGINT, sighandler);
	initEnvList(env, &head);
	hist_head = pull_hist(&hist_head, head);
	helper = initHelper(head, hist_head);
	/* grab the history file and populate the hist linked list */
	while (1)
	{
		if (argc == 1 && type == 1)
			prompt();
		inp = get_line(file, helper);
		args = NULL;
		while (inp != NULL)
		{
			tok = splitstr(inp, &delim, &save);
			if (checkBuiltins(inp, save, &head, helper) == 0)
			{
				if ((tok[0] == '.' && tok[1] == '/') || tok[0] == '/')
				{
					if (tok[0] == '.')
						tok = tok + 2;
					if (stat(tok, &st) == 0 &&
					    (st.st_mode & S_IXUSR) && S_ISREG(st.st_mode))
					{
						args = getArgs(tok, argv, save);
						cstatus = runProg(tok, args, head);
					}
					else
					{
						if (tok[0] != '/')
							tok = (tok - 2);
						_putstring(tok);
						_putstring(": No such file or directory.\n");
					}
				}
				else
				{
					cstatus = checkPath(tok, args, save, head);
					if (cstatus == 0)
					{
						_putstring(tok);
						_putstring(": command not found.\n");
					}
				}
			}
			inp = get_line(file, helper);
			save = NULL;
		}
		if (inp == NULL && (argc == 2 || type == 0))
		{
			free(inp);
			free_list(head);
			_exit(9);
		}
	}
}

/**
 * checkBuiltins - checks for builtin commands matching the first inputted word
 *
 * @inp: input string from main
 * @save: saveptr for tokens
 *
 * Return: returns 1 on success, 0 on failure.
 */
int checkBuiltins(char *inp, char *save, env_t **environ, helper_t *helper)
{
	char *value, *tok;
	char delim = ' ';
	hist_t *hist_head;

	hist_head = helper->hist_head;
	if (allstrcmp(inp, "env") == 0) /*if the first word is env, run env*/
	{
		listEnv(environ);
	}
	else if (allstrcmp(inp, "exit") == 0) /* probably split this into a dif func*/
	{
		tok = splitstr(NULL, &delim, &save);
		push_hist(helper->hist_head, helper->env);
		exitBuiltin(tok, inp, environ, helper);
	}
	else if (allstrcmp(inp, "setenv") == 0) /*setenv*/
	{
		tok = splitstr(NULL, &delim, &save);
		value = splitstr(NULL, &delim, &save);
		setEnvPtr(tok, value, *environ);
	}
	else if (allstrcmp(inp, "unsetenv") == 0)
	{
		tok = splitstr(NULL, &delim, &save);
		unsetEnv(tok, environ);
	}
	else if (allstrcmp(inp, "history") == 0)
		print_hist(hist_head);
	else if (allstrcmp(inp, "cd") == 0)
		cdBuiltin(save, *environ);
	else if (allstrcmp(inp, "help") == 0)
	{
		tok = splitstr(NULL, &delim, &save);
		helpBuiltIn(tok);
	}
	else
		return (0);

	return (1);
}

int getTermType(int file)
{
	struct stat st;
	fstat(file, &st);
	if (S_ISCHR(st.st_mode)) {
		return (1);
	} else if (S_ISFIFO(st.st_mode)) {
		return (0);
	}
	return (-1);
}

helper_t *initHelper(env_t *env, hist_t *hist_head)
{
	helper_t *helper;

	helper = malloc(sizeof(helper_t));
	helper->env = env;
	helper->hist_head = hist_head;
	helper->printed = malloc(sizeof(int) * 1);
	*(helper->printed) = 0;
	helper->total = malloc(sizeof(long) * 1);
	*(helper->total) = 0;
	helper->bufsize = malloc(sizeof(int) * 1);
	*(helper->bufsize) = 1024;
	helper->last = malloc(sizeof(int) * 1);
	*(helper->last) = 0;

	return (helper);
}


/**
 * checkPath - checks to see if a program is located in $PATH
 * if it is, will ruin the program and return 1
 *
 * @inp: input string we're working with
 * @argv: program argv
 * @save: splitstring save pointer
 *
 * Return: returns 1 if program ran, 0 if some sort of error
 */
int checkPath(char *inp, char *argv[], char *save, env_t *head)
{
	int j;
	char *temp, *path[PATHSIZE], *tok, **args, *pathsave, *paths, *cwd;
	char colon = ':';

	temp = NULL;
	cwd = malloc(100);
	getcwd(cwd, 100);
	if (getEnvPtr("PATH", head) != NULL)
	{
		if (inp == NULL || inp[0] == '\0')
			return (-1);
		j = 0;
		paths = _strdup((getEnvPtr("PATH", head))->value); /* tmp to avoid mangling env */
		tok = splitstr(paths, &colon, &pathsave);
		while (tok != NULL)
		{
			path[j++] = tok;
			tok = splitstr(NULL, &colon, &pathsave);
		}
		path[j] = NULL;
		tok = inp; j = 0;
		while (path[j] != NULL)
		{
			if (path[j][0] == '\0')
				temp = dir_concat(cwd, tok);
			else
				temp = dir_concat(path[j], tok);
			if (access(temp, X_OK) == 0)
			{
				args = getArgs(tok, argv, save);
				runProg(temp, args, head);
				break;
			}
			if (temp != NULL)
			{
				free(temp);
				temp = NULL;
			}
			j++;
		}
		if (path[j] == NULL)
		{
			if (temp != NULL)
				free(temp);
			free(paths);
			free(cwd);
			return (0); /* Need to free 2d array for path either way, on return 1 or 0! */
		}
	}
	free(paths);
	if (temp != NULL)
		free(temp);
	free(cwd);
	return (1);

}


/**
 * freeArgs - frees a 2d array
 *
 * @args: argument array
 */
void freeArgs(char **args, int envsize)
{
	int i;

	for (i = 0; i < envsize; i++)
	{
		free(args[i]);
	}
	free(args);
}


/**
 * getArgs - creates a 2d array of arguments
 * last argument will be null, first will be main's argv[0]
 *
 * @argv: argv for main
 * @save: saveptr for arguments
 * Return: returns a 2d array
 */
char **getArgs(char *tok, char *argv[], char *save)
{
	char **args;
	char *arg;
	char delim = ' ';
	int i;

/* Need a clever way to get the number of args before allocating
 * Currently this just allocates space out for 100 pntrs, that's... a lot
 */
	(void) argv;
	args = malloc(sizeof(char *) * 10000);
	arg = NULL;
	args[0] = tok;
	arg = splitstr(NULL, &delim, &save);
	i = 1;
	while (arg != NULL)
	{
		if (arg[0] != '\0')
			args[i++] = arg;
		arg = splitstr(NULL, &delim, &save);
	}
	args[i] = NULL;

	return (args);
}


/**
 * runProg - runs a program by forking and running it on the child
 *
 * @name: name of program (including whole path)
 * @argv: 2d array of arguments, either from main, or from getArgs
 * Return: returns -1 on failure, or the exit status of the child
 */
int runProg(char *name, char *argv[], env_t *head)
{
	pid_t cpid;
	int cstatus, envsize;
	char **envs;

	envs = buildEnv(head, &envsize);
	cpid = fork();
	if (cpid == -1) /* if fork returns -1, it failed */
	{
		_putstring("Failure to fork!");
		exit(9);
	}
	else if (cpid == 0) /* if fork return is 0, we are the child proc */
	{
		execve(name, argv, envs);
		_putstring("Attempted to run unknown command: ");
		_putstring(name);
		_putchar('\n');
		exit(-1);
	}
	else /* if neither are true, we're in the parent */
	{
		wait(&cstatus); /* wait til we get back */
		free(argv);
		freeArgs(envs, envsize);
		return (cstatus); /* now return the child's exit status */
	}

}


void sighandler(int signum)
{
	(void) signum;
	write(0, "\n", 1);
	prompt();
}
