#include "shell.h"

char *get_line(int file, hist_t **hist_head)
{
	char *line, *newbuf;
	int readval, i, bufsize;

	static int total = 0;
	static int printed = 0;
	static int last = 0;
	static char *buf, *bufhead;
	if (total == 0)
	{
		printed = 0; /* printed holds a count of what we've sent out*/
		bufsize = 1024; /*bufsize starts at 1024*/
		buf = malloc(sizeof(char) * bufsize);
		memset(buf, '\0', bufsize);
		readval = read(file, buf, 1024);
		total = readval; /*total is the total we've read, static*/
		while (readval >= 1024) /*if we read 1024, there's more in stdin*/
		{
			i = 1;
			newbuf = malloc((bufsize + 1024) * sizeof(char)); /*ghetto realloc*/
			memset(newbuf, '\0', bufsize + 1024);
			_memcpy(newbuf, buf, bufsize);
			free(buf);
			buf = newbuf; /*need to free buf here, gotta test more*/
			readval = read(STDIN_FILENO, buf + (bufsize), 1024); /*read more*/
			bufsize += 1024;
			total += readval; /*add the readval to the total we've read*/
			i++;
		}
		add_hist(total, hist_head, buf); /* create/add to history node */
		bufhead = buf; /*bufhead is a ptr to the beginning of the buffer*/
	}
	else
	{
		buf += last; /*if th is isn't the first time around, advance buf ptr*/
	}
	if (printed == total) /*if this is true, we're done with this buffer*/
	{
		printed = 0;
		total = 0;
		free(bufhead);
		return (NULL);
	}
	i = 0;
	while (i <= total) /*run through and find ';' and '\n'*/
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
	last = i + 1; /*this is where we need buf to be next, +1 for the '\0'*/
	printed += i + 1; /*total count on how many we've printed*/
	return (buf); /* return buf */
}
