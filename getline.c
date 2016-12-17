#include "shell.h"

char *get_line(int file, helper_t *helper)
{
	char *newbuf;
	int readval, i, *bufsize;
	hist_t **hist_head;
	static int *total;
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
		readval = read(file, buf, *bufsize);
		if (readval == -1)
		{
			_putstring("unable to read from STDIN_FILENO\n");
			return (NULL);
		}
		*total = readval; /*total is the total we've read, static*/
		while (readval == 1024) /*if we read 1024, there's more in stdin*/
		{
			_putstring("expand");
			newbuf = malloc((*bufsize * 2) * sizeof(char)); /*ghetto realloc*/
			memset(newbuf, '\0', *bufsize * 2);
			_memcpy(newbuf, buf, *bufsize);
			free(buf);
			buf = newbuf;
			readval = read(STDIN_FILENO, buf + *bufsize, 1024); /*read more*/
			if (readval == -1)
			{
				_putstring("unable to read from STDIN_FILENO past 1024 bytes\n");
				return (NULL);
			}
			*total += readval; /*add the readval to the total we've read*/
			*bufsize *= 2;
		}
		if (buf[0] != '\0')
			add_hist(*total, hist_head, buf);
		bufhead = buf; /*bufhead is a ptr to the beginning of the buffer*/
	}
	else
	{
		buf += *last; /*if this isn't the first time around, advance buf ptr*/
	}
	if (buf[0] == ';')
	{
		buf += 1;
		*bufsize -= 1;
	}
	if (*printed >= *total || buf[0] == '\n' || buf[0] == '\0') /*if this is true, we're done with this buffer*/
	{
		*printed = 0;
		*total = 0;
		free(bufhead);
		return (NULL);
	}
	i = 0;
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
	newbuf = parseDollar(buf, helper);
	if (newbuf != buf)
	{
		buf = newbuf;
		bufhead = newbuf;
	}
	buf = parseWhitespace(buf);
//	if (*last == *total && buf[0] == '\0')
	return (buf); /* return buf */
}

char *parseDollar(char *buf, helper_t *helper)
{
	char *name;
	char *newbuf;
	char *value;
	env_t *envname;
	int i, j, k, start, size;
	env_t *env;
	int *total;
 
	start = 0;
	env = helper->env;
	i = 0;
	total = helper->total;
	while (i < _strlen(buf))
	{
		if (buf[i] == '$')
		{
			start = i + 1;
			j = 0;
			i++;
			while (buf[i] != ' ' && buf[i] != '\n' && buf[i] != '\0' && buf[i] != '$')
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
				buf = newbuf;
			}
			else
			{
				value = envname->value;
				newbuf = sliceString(buf, helper->bufsize, _strlen(name) + 1, start - 1);
				newbuf = innerCat(newbuf, value, helper->bufsize, start - 1);
				*(helper->last) += (_strlen(value) - _strlen(name) - 1);
				buf = newbuf;
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
	return (newbuf);
}

char *expandBuffer(char *buf, int bufsize, int newsize)
{
	char *newbuf;

	newbuf = malloc(newsize * sizeof(char));
	memset(newbuf, '\0', newsize);
	_memcpy(newbuf, buf, bufsize);
	free(buf);
	buf = newbuf;
	return (buf);
}

/*
newbuf = malloc((*bufsize + 1024) * sizeof(char)); /*ghetto realloc*/
//memset(newbuf, '\0', *bufsize + 1024);
//_memcpy(newbuf, buf, *bufsize);
//free(buf);
//buf = newbuf; /*need to free buf here, gotta test more*/
//readval = read(STDIN_FILENO, buf + (*bufsize), 1024); /*read more*/
//*total += readval; /*add the readval to the total we've read*/
