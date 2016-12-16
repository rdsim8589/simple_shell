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
