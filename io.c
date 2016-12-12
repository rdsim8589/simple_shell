#include "shell.h"

/**
 * prompt - prints a prompt
 */
void prompt(void)
{
	_putstring(ANSI_COLOR_GREEN);
	_putstring(getcwd(NULL, 100));
	_putchar('$');
	_putchar(' ');
	_putstring(ANSI_COLOR_BLUE);
}

/**
 * get_line - gets a line of input from a source
 *
 * @file: file description
 * Return: returns a line of input as a single string
 */
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
