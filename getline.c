#include "shell.h"

char *get_line(int file, helper_t *helper)
{
	char *newbuf, *readbuf;
	long readval;
	int i, *bufsize;
	hist_t **hist_head;
	static long *total;
	static int *printed;
	static int *last;
	static char *buf, *bufhead;

	last = helper->last;
	total = helper->total;
	printed = helper->printed;
	hist_head = &helper->hist_head;
	bufsize = helper->bufsize; /*bufsize starts at 1024*/

	if (*total == 0)
	{
		*bufsize = 1024;
		*printed = 0; /* printed holds a count of what we've sent out*/
		buf = malloc(sizeof(char) * *bufsize);
		memset(buf, '\0', *bufsize);
		readval = read(file, buf, 1024);
		*total = readval; /*total is the total we've read, static*/
		while (readval >= 1024) /*if we read 1024, there's more in stdin*/
		{
			readbuf = malloc(1024);
			readval = read(file, readbuf, 1024); /*read more*/
			newbuf = malloc(*bufsize + 1024);
			memset(newbuf, '\0', *bufsize + 1024);
			newbuf = _memcpy(newbuf, buf, *bufsize);
			_memcpy(newbuf + *bufsize, readbuf, 1024);
			free(buf);
			buf = newbuf;
			free(readbuf);
			*total += readval; /*add the readval to the total we've read*/
			*bufsize += 1024;
		}
		if (buf[0] != '\0')
			add_hist(*total + 1, hist_head, buf);
		bufhead = buf; /*bufhead is a ptr to the beginning of the buffer*/
	}
	else
	{
		buf += *last; /*if this isn't the first time around, advance buf ptr*/
		if (buf[0] == '\0')
			buf++;
		if (*printed >= *total) /*if this is true, we're done with this buffer*/
		{
			*printed = 0;
			*total = 0;
			free(bufhead);
			return (NULL);
		}
		i = 0;
		while (buf[i] != '\0') /*figure out how many chars we're printing*/
		{
			i++;
		}
		*last = i + 1; /*this is where we need buf to be next, +1 for the '\0'*/
		*printed += i + 1; /*total count on how many we've printed*/
		return(buf);
	}
	if (buf[0] == ';')
	{
		buf += 1;
		*bufsize -= 1;
	}
	i = 0;
	buf = parseDollar(buf, helper);
	while (i < *total) /*run through and find ';' and '\n'*/
	{
		if (buf[i] == EOF)
		{
			buf[i] = '\0';
		}
		if (buf[i] == ';')
		{
			buf[i] = '\0';
		}
		else if (buf[i] == '\n')
		{
			buf[i] = '\0';
		}
		i++;
	}
	i = 0;
 	while (buf[i] != '\0') /*figure out how many chars we're printing*/
	{
		i++;
	}
	*last = i + 1; /*this is where we need buf to be next, +1 for the '\0'*/
	*printed += i + 1; /*total count on how many we've printed*/
	bufhead = buf;
//	buf = parseWhitespace(buf);
	return (buf); /* return buf */
}

char *parseDollar(char *buf, helper_t *helper)
{
	char *name;
	char *newbuf;
	char *value;
	env_t *envname;
	int i, j, k, start, change;
	env_t *env;

	start = 0;
	env = helper->env;
	i = 0;
	change = 0;
	while (i < _strlen(buf))
	{
		if (buf[i] == '$')
		{
			start = i + 1;
			j = 0;
			i++;
			while (buf[i] != ' ' && buf[i] != '\n' && buf[i] != '\0' && buf[i] != '$' && buf[i] != ';')
			{
				i++;
				j++;
			}
			name = malloc((j + 1) * (sizeof(char)));
			j = 0;
			k = start;
			while (k != i)
			{
				name[j++] = buf[k++];
			}
			name[j] = '\0';
			envname = getEnvPtr(name, env);
			if (envname == NULL)
			{
				newbuf = sliceString(buf, helper->bufsize, _strlen(name) + 1, start - 1);
				free(buf);
				buf = newbuf;
			}
			else
			{
				value = envname->value;
				newbuf = sliceString(buf, helper->bufsize, _strlen(name) + 1, start - 1);
				buf = innerCat(newbuf, value, helper->bufsize, start - 1);
				*(helper->total) += _strlen(value);
				*(helper->total) -= (_strlen(name) + 1);
			}
			free(name);
		}
		if (start != 0)
		{
			i = start;
			start = 0;
		}
		i++;
	}
	return (buf);
}

char *parseWhitespace(char *buf)
{
	int length;
	while (buf[0] == ' ')
		buf++;
	length = _strlen(buf);
	if (length != 0)
	{
		while (buf[length - 1] == ' ')
		{
			buf[length - 1] = '\0';
			length = _strlen(buf);
		}
	}
	return (buf);
}

/**
 * innerCat - 'concatenates' a string to the inside of a buffer
 * after remallocing it large and freeing old buffer.
 *
 * @buf: Buffer to insert string into.
 * @string: Null terminated string to insert.
 * @bufsize: Size of buffer.
 * @insert: Index of buffer where string is to be inserted.
 *
 * Return: returns the resized buffer.
 */
char *innerCat(char *buf, char *string, int *bufsize, int insert)
{
	char *newbuf;
	int newsize;

	newsize = *bufsize + _strlen(string);

	newbuf = malloc(newsize * sizeof(char));
	memset(newbuf, '\0', newsize);
	_memcpy(newbuf, buf, insert);
	_memcpy(newbuf + insert, string, _strlen(string));
	_memcpy(newbuf + insert  + _strlen(string), buf + insert, *bufsize - insert);
	*bufsize = newsize;
	free(buf);
	return (newbuf);
}

/**
 * sliceString - slices a certain number of characters from a buffer
 * reallocs the buffer to a smaller size and frees it automatically.
 *
 * @buf: buffer
 * @bufsize: size of buffer
 * @slicesize: number of characters to remove
 * @index: index of where to start slicing
 *
 * Return: Returns a pointer to the newly sliced string, or NULL if not possible.
 */
char *sliceString(char *buf, int *bufsize, int slicesize, int index)
{
	char *newbuf;
	int newsize;

	newsize = *bufsize - slicesize;
	newbuf = malloc(newsize * sizeof(char));
	memset(newbuf, '\0', newsize);
	_memcpy(newbuf, buf, index);
	if (buf[index + slicesize] != '\0')
		_memcpy(newbuf + index, buf + index + slicesize, *bufsize - index - slicesize);
	*bufsize = newsize;
	free(buf);
	return (newbuf);
}

char *expandBuffer(char *buf, int bufsize, int newsize)
{
	char *newbuf;

	newbuf = malloc(newsize * sizeof(char));
	memcpy(newbuf, buf, bufsize);
	free(buf);
	buf = newbuf;
	return (buf);
}
