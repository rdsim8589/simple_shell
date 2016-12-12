#include "shell.h"

/**
 * main - entry point for shell program
 * loops input for a shell, splits them into appropriate actions
 *
 * @argc: argument count
 * @argv: arguments passed
 * Return: return values in man page
 */
int main(int argc, char *argv[])
{
	char *save, *tok, *inp, **args;
	char delim = ' ';

	(void) argc; /* need to use this to check to check for scripts*/
	signal(SIGINT, SIG_IGN); /* Ignore any SIGINT (ctrl-c) signal */
	while (1)
	{
		prompt();
		inp = get_line(STDIN_FILENO);
		args = NULL;
		if (inp != NULL)
		{
			tok = splitstr(inp, &delim, &save);
			if (checkBuiltins(inp, save) == 0)
			{
				if (tok[0] == '.' && tok[1] == '/')
				{
					tok = tok + 2;
					if (access(tok, X_OK) == 0)
					{
						args = getArgs(tok, argv, save);
						runProg(tok, args);
					}
					else
					{
						tok = (tok - 2);
						_putstring(tok);
						_putstring(": No such file or directory.\n");
					}
				}
				else
					if (checkPath(tok, argv, save) == 0)
					{
						_putstring(tok); _putstring(": command not found.\n");
					}
			}
			free(inp);
			save = NULL;
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
int checkPath(char *inp, char *argv[], char *save)
{
	int j;
	char *temp, *path[PATHSIZE], *tok, **args, *pathsave;
	char delim2 = '=';
	char colon = ':';

	if (getEnvPtr("PATH") != NULL)
	{
		j = 0;
		temp = _strdup(getEnvPtr("PATH")); /* tmp to avoid mangling env */
		tok = splitstr(temp, &delim2, &pathsave);
		if (tok != NULL)
			tok = splitstr(NULL, &colon, &pathsave);
		while (tok != NULL)
		{
			path[j++] = tok;
			tok = splitstr(NULL, &colon, &pathsave);
		}
		path[j] = NULL;
		free(temp);
		tok = inp; j = 0;
		while (path[j] != NULL)
		{
			temp = dir_concat(path[j], tok);
			if (access(temp, X_OK) == 0)
			{
				args = getArgs(tok, argv, save);
				runProg(temp, args);
				break;
			}
			free(temp);
			j++;
		}
		if (path[j] != NULL)
		{
			free(temp);
			freeArgs(args);
			return (1); /* Need to free 2d array for path either way, on return 1 or 0! */
		}
	}
	return (0);

}


/**
 * freeArgs - frees a 2d array
 *
 * @args: argument array
 */
void freeArgs(char **args)
{
	int i;

	i = 0;
	while (args[i] != NULL)
	{
		free(args[i]);
		i++;
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
	args = malloc(sizeof(char *) * 100);
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
int runProg(char *name, char *argv[])
{
	pid_t cpid;
	int cstatus;

	cpid = fork();
	if (cpid == -1) /* if fork returns -1, it failed */
	{
		_putstring("Failure to fork!");
		exit(9);
	}
	else if (cpid == 0) /* if fork return is 0, we are the child proc */
	{
		execve(name, argv, environ);
		_putstring("Attempted to run unknown command: ");
		_putstring(name);
		_putchar('\n');
		return (-1);
	}
	else /* if neither are true, we're in the parent */
	{
		wait(&cstatus); /* wait til we get back */
		free(argv);
		return (cstatus); /* now return the child's exit status */
	}

}
