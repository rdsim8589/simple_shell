#include "shell.h"

/**
 * main - entry point for shell program
 * loops input for a shell, splits them into appropriate actions
 *
 * @argc: argument count
 * @argv: arguments passed
 * Return: return values in man page
 */
int main(int argc, char *argv[], char*env[])
{
	char *save, *tok, *inp, **args;
	char delim = ' ';
	env_t *head;
	int file;
	hist_t *hist_head;

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
	(void) argc; /* need to use this to check to check for scripts later!*/
	signal(SIGINT, SIG_IGN); /* Ignore any SIGINT (ctrl-c) signal */
	initEnvList(env, &head);
	while (1)
	{
		if (argc == 1)
		    prompt();
		inp = get_line(file, &hist_head);
		args = NULL;
	        while (inp != NULL)
		{
			tok = splitstr(inp, &delim, &save);
			if (checkBuiltins(inp, save, &head , &hist_head) == 0)
			{
				if (tok[0] == '.' && tok[1] == '/')
				{
					tok = tok + 2;
					if (access(tok, X_OK) == 0)
					{
						args = getArgs(tok, argv, save);
						runProg(tok, args, head);
					}
					else
					{
						tok = (tok - 2);
						_putstring(tok);
						_putstring(": No such file or directory.\n");
					}
				}
				else
					if (checkPath(tok, args, save, head) == 0)
					{
						_putstring(tok); _putstring(": command not found.\n");
					}
			}
			inp = get_line(file, &hist_head);
			save = NULL;
		}
		if (inp == NULL && argc == 2)
		{
			free(inp);
			free_list(head);
			_exit(9);
		}
	}
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
	char *temp, *path[PATHSIZE], *tok, **args, *pathsave, *paths;
	char colon = ':';

	if (getEnvPtr("PATH", head) != NULL)
	{
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
		if (path[j] != NULL)
		{
			if (temp != NULL)
				free(temp);
			free(paths);
			return (1); /* Need to free 2d array for path either way, on return 1 or 0! */
		}
	}
	free(paths);
	free(temp);
	return (0);

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
	args = malloc(sizeof(char *) * 1000);
	arg = NULL;
	args[0] = tok;
	arg = splitstr(NULL, &delim, &save);
	i = 1;
	while (arg != NULL)
	{
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
		return (-1);
	}
	else /* if neither are true, we're in the parent */
	{
		wait(&cstatus); /* wait til we get back */
		free(argv);
		freeArgs(envs, envsize);
		return (cstatus); /* now return the child's exit status */
	}

}
