#include "shell.h"

/**
 * prompt - prints a prompt
 */
void prompt(void)
{
	char *cwd;

	cwd = getcwd(NULL, 100);
	_putstring(ANSI_COLOR_GREEN);
	_putstring(cwd);
	_putchar('$');
	_putchar(' ');
	_putstring(ANSI_COLOR_RED);
	free(cwd);
}

/**
 * get_line - gets a line of input from a source
 *
 * @file: file description
 * Return: returns a line of input as a single string
 */
char *get_line(const int file, int *buf_full)
{
	char buf[1024], *line_buf;
	int i, j, readval;
	static int readval_num = 0;
	static int buf_i = 0;

	if (buf == NULL)
		return (NULL);
	if (*buf_full == 1)
	{
		readval = read(file, buf, BUFSIZE);

		if (readval == 1)
			return (NULL);
		if (readval == -1)
			return (NULL);
		if (readval >= BUFSIZE)
			/* realloc space for buf and read file again */
			return (NULL);
		readval_num += readval;
	}
	j = 0;
	for (i = buf_i; buf[i] != ';' && buf[i] != '\n'; i++)
	{
		j++;
	}

	/* considering mallocing more space to account for expansion */
	line_buf = malloc((j + 1) * sizeof(char));
	i = (i - buf_i);

	for (j = 0; j <= i; j++)
		/* here is where to deal with expansion */
	{
		line_buf[j] = buf[buf_i++];
	}
	line_buf[j - 1] = '\0';
	/* needs to read again if BUFSIZE is exceeded */
	if (buf_i >= readval - 1)
	{
		_putstring("get_line: enter buf_full\n");
		buf_i = 0;
		*buf_full = 0;
	}
	else
	{
		(*buf_full)++;
	}
	return (line_buf);
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
