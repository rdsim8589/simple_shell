#include "shell.h"

/**
 * whitespace - squeezes whitespace from a buffer
 *
 * @buf: buffer
 * @helper: helper struct
 * Return: returns a squeez'd buffer
 */
char *whitespace(char *buf, helper_t *helper)
{
	unsigned int i;

	for (i = 0; i <= *helper->total && buf[i] != '\0'; i++)
	{
		for (i = i; buf[i] == ' '; i++)
		{
			if (i == 0 || buf[i - 1] == '\n')
				while (buf[i] == ' ')
					buf = bufferDelete(buf, helper, i, 1);
			if (i == 0 && (buf[i + 1] == ';' || buf[i + 1] == '\0' ||
				       buf[i + 1] == ' ' || buf[i + 1] == '\n'))
			{
				while (buf[i] == ' ')
					buf = bufferDelete(buf, helper, i, 1);
			}
			else if (i > 0 && buf[i] == ' '  && buf[i - 1] == ';')
				while (buf[i] == ' ')
					buf = bufferDelete(buf, helper, i, 1);
		}
	}
	return (buf);
}

/**
 * parseDollar - parses dollarsigns from a buffer
 *
 * @buf: buffer
 * @helper: helper struct
 *
 * Return: returns a parsed buffer
 */
char *parseDollar(char *buf, helper_t *helper)
{
	char *name, *newbuf, *value;
	env_t *envname, *env;
	int i, j, k, start;

	start = 0; env = helper->env;
	for (i = 0; i < _strlen(buf); i++)
	{
		if (buf[i] == '$')
		{
			start = i + 1; j = 0; i++;
			while (!isDelimiter(buf[i]) && (!isWhitespace(buf[i]))
			       && (_isalphanum(buf[i])) && buf[i] != '$' && buf[i] != '?')
			{
				i++; j++;
			}
			if (j != 0)
			{
				name = malloc((j + 1) * (sizeof(char)));
				j = 0;
				k = start;
				while (k != i)
					name[j++] = buf[k++];
				name[j] = '\0';
				envname = getEnvPtr(name, env);
				if (envname == NULL)
					buf = sliceString(buf, helper->bufsize, _strlen(name) + 1, start - 1);
				else
				{
					value = envname->value;
					newbuf = sliceString(buf, helper->bufsize, _strlen(name) + 1, start - 1);
					buf = innerCat(newbuf, value, helper->bufsize, start - 1);
				}
				free(name);
			}
		}
		if (start != 0)
		{
			i = start;
			start = 0;
		}
	}
	return (buf);
}

/**
 * parseComments - parses the comment portion of buffer
 * @buf: the buffer
 * @helper: the helper struct
 *
 * Return: the parsed section of the buffer
 */
char *parseComments(char *buf, helper_t *helper)
{
	int i;

	for (i = 0; i < _strlen(buf); i++)
	{
		if (buf[i] == '#')
		{
			if (i == 0 || isDelimiter(buf[i - 1]) || buf[i - 1] == ' ')
			{
				while (!isDelimiter(buf[i]) && _strlen(buf) > i)
				{
					buf = sliceString(buf, helper->bufsize, 1, i);
				}
			}
		}
	}
	return (buf);
}
/**
 * bufferDelete - remove parts of the buffer
 * @buf: the buffer
 * @helper: ptr to helper struct
 * @index: index to remove parts of the buffer
 * @times: 1 to remove one character
 *
 * Return: ptr to the buffer
 */
char *bufferDelete(char *buf, helper_t *helper, int index, int times)
{
	char *newbuf;

	newbuf = sliceString(buf, helper->bufsize, times, index);
	buf = newbuf;

	return (buf);
}
/**
 * isDelimiter - determines if a character is a delimiter
 * @c: a character
 *
 * Return: 1 if true, 0 if false
 */
int isDelimiter(char c)
{
	if (c == ';' || c == '\n' || c == '\0')
		return (1);
	else
		return (0);
}
/**
 * isWhitespace -  determines if a character is ' ', '\n', or '\t'
 * @c: a character
 *
 * Return: 1 if found and 0 if no found
 */
int isWhitespace(char c)
{
	if (c == ' ' || c == '\n' || c == '\t')
		return (1);
	else
		return(0);
}

/**
 * _getpid - gets the PID of the currently running process
 * Return: returns a string containing the PID
 */
char *_getpid(void)
{
	pid_t pid;
	char *cpid, *dir, *buf, *save, *tok, *newbuf;
	int file, status, readval;

	pid = fork();
	if (pid > 0)
	{
		cpid = malloc(sizeof(long) * sizeof(char));
		cpid[0] = '\0';
		dir = malloc(100);
		dir[0] = '\0';
		itoa(pid, cpid);
		_strcat(dir, "/proc/");
		_strcat(dir, cpid);
		_strcat(dir, "/stat");
		file = open(dir, O_RDONLY);
		buf = malloc(1024);
		readval = read(file, buf, 1024);
		if (readval < 0)
		{
			perror("Error reading from /proc/: ");
			return (NULL);
		}
		tok = splitstr(buf, " ", &save);
		tok = splitstr(NULL, " ", &save);
		tok = splitstr(NULL, " ", &save);
		tok = splitstr(NULL, " ", &save);
		wait(&status);
	}
	else
	{
		_exit(0);
	}
	free(dir);
	free(cpid);
	newbuf = malloc(_strlen(tok) + 1);
	_memcpy(newbuf, tok, _strlen(tok) + 1);
	free(buf);
	return (newbuf);
}

/**
 * _isalphanum - returns 1 if number is an alphanumeric character, 0 if not
 *
 * @c: int to check for alphanumericalness
 * Return: 1 if alphanumeric, 0 otherwise.
 */
int _isalphanum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
		return (1);
	else
		return (0);
}
