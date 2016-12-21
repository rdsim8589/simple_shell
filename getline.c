#include "shell.h"


/**
 * get_line - gets a line from given file
 *
 * @file: file passed
 * @helper: helper_t helper struct
 *
 * Return: returns a pointer to the new buff
 */
char *get_line(int file, helper_t *helper)
{
	char *newbuf, *readbuf, *buf;
	long readval;

	buf = malloc(sizeof(char) * (*helper->bufsize));
	_memset(buf, '\0', (*helper->bufsize));
	readval = read(file, buf, *helper->bufsize);
	(*helper->total) = readval;
	if (readval == 0)
		exitBuiltin("0", buf, helper);
	while (readval >= 1024)
	{
		readbuf = malloc(1024);
		readval = read(file, readbuf, 1024);
		newbuf = malloc((*helper->bufsize) + 1024);
		_memset(newbuf, '\0', (*helper->bufsize) + 1024);
		newbuf = _memcpy(newbuf, buf, (*helper->bufsize));
		_memcpy(newbuf + (*helper->bufsize), readbuf, 1024);
		free(buf); buf = newbuf; free(readbuf);
		(*helper->total) += readval; (*helper->bufsize) += 1024;
	}
	buf[(*helper->total - 1)] = '\0';
	if (buf[0] != '\0')
		add_hist((*helper->total + 1), &helper->hist_head, buf);
	buf = whitespace(buf, helper);
	buf = parseComments(buf, helper);
	buf = parseDollar(buf, helper);
	countBuf(buf, helper);
	parseDelimiters(buf, helper);
	helper->bufhead = buf;
	return (buf);
}
/**
 * parseDelimiters - replace delimiters with '\0'
 * @buf: the buffer
 * @helper: the helper struct
 *
 * Return: the buffer
 */
char *parseDelimiters(char *buf, helper_t *helper)
{
	int i;

	for (i = 0; i < *helper->total; i++)
	{
		if (buf[i] == EOF)
			buf[i] = '\0';
		if (buf[i] == ';')
			buf[i] = '\0';
		else if (buf[i] == '\n')
			buf[i] = '\0';
	}
	return (buf);
}
/**
 * countBuf - count and save the length of buf after parse comments,
 * parse the dollar and removed excess white space.
 * @buf: buffer after it's been expanded
 * @helper: the helper struct
 */
void countBuf(char *buf, helper_t *helper)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
		;
	(*helper->total) = i;
	(*helper->printed) = 0;
}
/**
 * countLine - counts the len of a line after delimiter have been
 * replaced with '\0'
 * @buf: buffer
 * @helper: the helper struct
 *
 */
void countLine(char *buf, helper_t *helper)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
		;
	(*helper->last) = i + 1;
	(*helper->printed) += i + 1;
}


/**
 * moreLines - helper func for getline. Ran to check to see if there is
 * unprinted lines left in the buffer.
 *
 * @helper: helper struct
 * @buf: buffer
 * Return: Returns a pointer to the next line, or NULL if there are no more
 */
char *moreLines(helper_t *helper, char *buf)
{
	while (buf[0] == ';')
	{
		buf += 1;
		(*helper->total) -= 1;
		(*helper->printed) -= 1;
	}
	buf += (*helper->last);
	if (buf[0] == '\0')
		buf++;
	if ((*helper->printed) >= (*helper->total))
	{
		(*helper->printed) = 0;
		(*helper->total) = 0;
		free(helper->bufhead);
		return (NULL);
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
	_memset(newbuf, '\0', newsize);
	_memcpy(newbuf, buf, insert);
	_memcpy(newbuf + insert, string, _strlen(string));
	_memcpy(newbuf + insert  + _strlen(string), buf + insert, *bufsize - insert);
	*bufsize = newsize;
	free(buf);
	return (newbuf);
}

/**
 * sliceString - Cuts a substring out of a string and resizes it
 *
 * @buf: buffer to work with
 * @bufsize: size of buffer
 * @slicesize: size of the slice to be taken out
 * @index: where to slice in the buffer
 *
 * Return: Returns a resized buffer.
 */
char *sliceString(char *buf, int *bufsize, int slicesize, int index)
{
	char *newbuf;
	int newsize;

	newsize = *bufsize - slicesize;
	newbuf = malloc(newsize * sizeof(char));
	_memset(newbuf, '\0', newsize);
	_memcpy(newbuf, buf, index);
	if (buf[index + slicesize] != '\0')
	{
		_memcpy(newbuf + index, buf + index + slicesize,
			*bufsize - index - slicesize);
	}
	*bufsize = newsize;
	free(buf);
	return (newbuf);
}
