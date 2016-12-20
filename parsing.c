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

			if (i == 0 && (buf[i + 1] == ';' || buf[i + 1] == '\0' ||
				       buf[i + 1] == ' ' || buf[i + 1] == '\n'))
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
					*helper->total = *helper->total - 1;
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

char *_getpid(void)
{
	pid_t pid, pid2;
	char *cpid, *dir, *buf, *save, *tok, *newbuf;
	int file, status, status2, i, readval;

	pid = fork();
	if (pid > 0)
	{
		cpid = malloc(sizeof(long) * sizeof(char));
		cpid[0] = '\0';
		dir = malloc(100);
		dir[0] = '\0';
		itoa(pid, cpid);
		strcat(dir, "/proc/");
		strcat(dir, cpid);
		strcat(dir, "/stat");
		file = open(dir, O_RDONLY);
		buf = malloc(1024);
		readval = read(file, buf, 1024);
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
	memcpy(newbuf, tok, _strlen(tok) + 1);
	free(buf);
	return (newbuf);
}
