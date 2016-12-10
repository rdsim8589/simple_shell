#include "shell.h"

#define BUFSIZE 1024
#define ARGS 256

int main(int argc, char *argv[], char *envp[])
{
	char buf[BUFSIZE];
	char *save, *tok, *walk, *inp, *arg, *args[ARGS];
	char delim = ' ';
	int i, readval;
	i = 0;

	inp = get_line(STDIN_FILENO);
	if (inp != NULL)
	{
		tok = splitstr(inp, &delim, &save);
		printf("%s", tok);
		arg = splitstr(NULL, &delim, &save);
		printf("%s", arg);
	}
}


int runProg(char *name, char *argv[], char *envp[])
{
	pid_t cpid, rpid;
	int cstatus;

	cpid = fork();
	if (cpid == -1) /* if fork returns -1, it failed */
	{
		_putstring("Failure to fork!");
		exit (9);
	}
	else if (cpid == 0) { /* if fork return is 0, we are the child proc */
		execve(name, argv, envp);
		_putstring("Attempted to run unknown command.\n");  /* execve should never return! */
		return (-1);
	}
	else /* if neither are true, we're in the parent */
	{
		rpid = wait(&cstatus); /* wait til we get back */
		return (cstatus); /* now return the child's exit status */
	}

}

/**
 * splitstr - splits a string into tokens by deliminator
 * first call requires a string, all subsequent calls
 * require NULL to get more tokens from saved string
 *
 * @str: string to tokenize
 * @delim: deliminator
 * @saveptr: ptr used to save locations
 */
char *splitstr(char *str, const char *delim, char **saveptr)
{
	char de;
	char *tok;
	int i;

	if (str == NULL) /*if str is null, we check saveptr*/
	{
		if (*saveptr == NULL) /*if saveptr is also null, we're done*/
			return (NULL);
		str = *saveptr; /*if saveptr isn't null, let's drag it back up */
	}
	i = 0;
	tok = str; /* tok will start where str starts now */
	while (*str != '\0' && *str != *delim) /* move forward until we find a delim or null */
	{
		str++; /* we could check here for additional delims each time we rotate! */
		i++; /* for multiple delim setup (when we want to use ;, etc) */
	}
	if (*str == '\0') /* null found, this is our last word */
		*saveptr = NULL;
	else
		*saveptr = str + 1; /* save the letter after the null we just placed for next time */
	tok[i] = '\0';
	return (tok); /* return a pointer to our new token */
}


/**
 * allstrcmp - compares two strings, INCLUDING null terminator!
 *
 *
 * @s1: first string
 * @s2: second string
 * Return: difference in ASCII value between string 1 and string 2
 */
int allstrcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	if (s1[i] == '\0' || s2[i] == '\0')
		return (-1);
	while (s1[i] != '\0' && s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return ((s1[i] - '0') - (s2[i] - '0'));
		i++;
	}
	if (s1[i] == '\0' && s2[i] == '\0')
		return (0);
	return (i);
}

void prompt(void)
{
	_putchar('$');
	_putchar(' ');
}

char *get_line(const int file)
{
	char *buf;
	int readval;

	buf = malloc(BUFSIZE * sizeof(char));
	if (buf == NULL)
		return (NULL);
	readval = read(file, buf, BUFSIZE);
	if (readval == 1)
		return (NULL);
	if (readval == -1)
		return (NULL);
	buf[readval - 1] = '\0';
	return (buf);
}

/**
 * _putstring - prints a string to stdout, without a newline
 *
 * @str: string to print
 * Return: void
 */

void _putstring(char *str)
{
	int i;

	if (str == NULL)
		return;
	i = 0;
	while (str[i] != '\0')
		_putchar(str[i++]);
}

/**
 * _putchar - writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _putchar(char c)
{
	return (write(1, &c, 1));
}
