#include "shell.h"

char *whitespace(char *buf, helper_t *helper)
{
	unsigned int i;
	char *newbuf;


	newbuf = NULL;
	i = 0;
	while (i <= *helper->total && buf[i] != '\0')
	{
		while (buf[i] == ' ')
		{
			if (i == 0 || buf[i - 1] == '\n')
				while (buf[i] == ' ')
				{
					newbuf = sliceString(buf, helper->bufsize, 1, i);
					*helper->total = *helper->total - 1;
					*helper->printed = *helper->printed - 1;
					buf = newbuf;
				}

			if (i == 0 && (buf[i + 1] == ';' || buf[i + 1] == '\0'|| buf[i + 1] == ' ' || buf[i + 1] == '\n'))
			{
				while (buf[i] == ' ')
				{
					newbuf = sliceString(buf, helper->bufsize, 1, i);
					*helper->total = *helper->total - 1;
					*helper->printed = *helper->printed - 1;
					buf = newbuf;
				}
			}
			else if (i > 0 && buf[i] == ' '  && buf[i - 1] == ';')
			{
				while (buf[i] == ' ')
				{
					newbuf = sliceString(buf, helper->bufsize, 1, i);
					*helper->total= *helper->total - 1;
					*helper->printed = *helper->printed - 1;
					buf = newbuf;
				}
			}
			i++;
		}
	i++;
	}

	return (buf);
}
