#include "shell.h"
/**
 * main - entry point for shell program
 * loops input for a shell, splits them into appropriate actions
 *
 * @argc: argument count
 * @argv: arguments passed
 * @envp: list of environment names and their values
 *
 * Return: return values in man page
 */
int main(int argc, char *argv[], char *envp[])
{
	helper_t *helper;
	char *save, *tok, *inp, **args;
	char delim = ' ';

	helper = setupMain(argc, argv, envp);
	signal(SIGINT, sighandler);
	while (1)
	{
		if (argc == 1 && helper->type == 1)
			prompt();
		inp = get_line(helper->file, helper);
		args = NULL;
		while (inp != NULL)
		{
			countLine(inp, helper);
			tok = splitstr(inp, &delim, &save);
			if (checkBuiltins(inp, save, helper) == 1)
			{
				if (checkLocal(inp, helper, save) == 1)
				{
					helper->lastExit = checkPath(tok, args, save, helper);
					if (helper->lastExit == 1)
					{
						_putstring(tok);
						_putstring(": command not found.\n");
					}
				}
			}
			inp = moreLines(helper, inp);
			save = NULL;
		}
		if (inp == NULL && (argc == 2 || helper->type == 0))
		{
			free(inp);
			free_list(helper->env);
			_exit(9);
		}
	}
}

/**
 * setupMain - populates helper struct, determine if given STDIN, or File,
 * initialize the hist and env struct, grab the parent pid, and determines if
 * file in termianl or a pipe.
 * @argc: arugment count
 * @argv: arguements passed
 * @envp: list of environment names and their values
 *
 * Return: the helper struct
 */
helper_t *setupMain(int argc, char **argv, char **envp)
{
	char *pid;
	env_t *head;
	hist_t *hist_head;
	helper_t *helper;

	pid = _getpid();
	hist_head = NULL;
	head = NULL;

	if (argc > 2 || argv == NULL || envp == NULL)
	{
		_putstring("Please run with no argument, or one argument to run from script.");
		_exit(9);
	}

	initEnvList(envp, &head);
	hist_head = pull_hist(&hist_head, head);
	helper = initHelper(head, hist_head, pid);

	if (argc == 1)
		helper->file = STDIN_FILENO;
	else if (argc == 2)
	{
		helper->file = open(argv[1], O_RDONLY);
		if (helper->file == -1)
		{
			_putstring("Error opening script file: ");
			_putstring(argv[1]);
			_putchar('\n');
			_exit(9);
		}
	}

	helper->type = getTermType(helper->file);

	return (helper);
}


/**
 * checkLocal - checking if local
 * @tok: tokenized input
 * @helper: helper struct
 * @save: ptr of the token after @tok
 *
 * Return: 0 if success, 1 if fail, and 127 if no such file found
 */
int checkLocal(char *tok, helper_t *helper, char *save)
{
	struct stat st;
	char **args;

	args = NULL;
	if ((tok[0] == '.' && tok[1] == '/') || tok[0] == '/')
	{
		if (tok[0] == '.')
			tok = tok + 2;
		if (stat(tok, &st) == 0 &&
		    (st.st_mode & S_IXUSR) && S_ISREG(st.st_mode))
		{
			args = getArgs(tok, args, save);
			helper->lastExit = runProg(tok, args, helper);
			return(0);
		}
		else
		{
			if (tok[0] != '/')
				tok = (tok - 2);
			_putstring(tok);
			_putstring(": No such file or directory.\n");
			return (127);
		}
	}
	return (1);
}

/**
 * checkBuiltins - checks for builtin commands matching the first inputted word
 *
 * @inp: input string from main
 * @save: saveptr for tokens
 * @helper: the ptr to the helper struct
 *
 * Return: returns 0 on success, 1 on failure.
 */
int checkBuiltins(char *inp, char *save, helper_t *helper)
{
	char *value, *tok;
	char delim = ' ';
	hist_t *hist_head;

	hist_head = helper->hist_head;
	if (allstrcmp(inp, "env") == 0)
		listEnv(&helper->env);
	else if (allstrcmp(inp, "exit") == 0)
	{
		tok = splitstr(NULL, &delim, &save);
		push_hist(helper->hist_head, helper->env);
		exitBuiltin(tok, inp, helper);
	}
	else if (allstrcmp(inp, "setenv") == 0)
	{
		tok = splitstr(NULL, &delim, &save);
		value = splitstr(NULL, &delim, &save);
		setEnvPtr(tok, value, helper->env);
	}
	else if (allstrcmp(inp, "unsetenv") == 0)
	{
		tok = splitstr(NULL, &delim, &save);
		unsetEnv(tok, &helper->env);
	}
	else if (allstrcmp(inp, "history") == 0)
		print_hist(hist_head);
	else if (allstrcmp(inp, "cd") == 0)
		cdBuiltin(save, helper);
	else if (allstrcmp(inp, "help") == 0)
	{
		tok = splitstr(NULL, &delim, &save);
		helpBuiltIn(tok);
	}
	else
		return (1);

	return (0);
}
/**
 * getTermType - checks if file is in terminal, or if file is has a pipe
 * @file: typically STDIN_FILENO
 *
 * Return: 1 if in terminal, 0 if pipe and -1 if fail
 */
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
/**
 * initHelper - fills and intialize values of the helper struct
 * @env: the head of the env linked list
 * @hist_head: the head of the hist linked list
 * @pid: the pid of the parents process
 *
 * Return: the ptr of the struct helper
 */
helper_t *initHelper(env_t *env, hist_t *hist_head, char *pid)
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
	helper->pid = pid;
	helper->lastExit = 0;

	return (helper);
}


/**
 * checkPath - checks to see if a program is located in $PATH
 * if it is, will ruin the program and return 1
 *
 * @inp: input string we're working with
 * @argv: program argv
 * @save: splitstring save pointer
 * @helper: ptr to the helper struct
 *
 * Return: returns 1 if program ran, 0 if some sort of error
 */
int checkPath(char *inp, char *argv[], char *save, helper_t *helper)
{
	int j;
	char *temp, *path[PATHSIZE], *tok, **args, *pathsave, *paths, *cwd;
	char colon = ':';

	temp = NULL;
	cwd = malloc(100);
	getcwd(cwd, 100);
	if (getEnvPtr("PATH", helper->env) != NULL)
	{
		if (inp == NULL || inp[0] == '\0')
		{
			free(cwd);
			return (-1);
		}
		j = 0;
		paths = _strdup((getEnvPtr("PATH", helper->env))->value); /* tmp to avoid mangling env */
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
				helper->lastExit = runProg(temp, args, helper);
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
			return (1); /* Need to free 2d array for path either way, on return 1 or 0! */
		}
	}
	free(cwd);
	free(paths);
	if (temp != NULL)
		free(temp);
	return (0);

}


/**
 * freeArgs - frees a 2d array
 *
 * @args: argument array
 * @envsize: the size of the array holding the environment variables
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
 * @tok: tokenized input
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
 * @helper: ptr to the helper struct
 * 
 * Return: returns -1 on failure, or the exit status of the child
 */
int runProg(char *name, char *argv[], helper_t *helper)
{
	pid_t cpid;
	int cstatus, envsize;
	char **envs;

	envs = buildEnv(helper->env, &envsize);
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
/**
 * sighandler - the signal handler
 * @signum: a signal number corresponding to the signal
 */
void sighandler(int signum)
{
	(void) signum;
	write(0, "\n", 1);
	prompt();
}
