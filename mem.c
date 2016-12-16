#include "shell.h"

/**
 * _realloc - moves data in pointer to new area with different space allocated
 *
 * @ptr: pointer to move data from
 * @old_size: size of current pointer
 * @new_size: size of new pointer
 * Return: returns pointer to space allocated
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *newspace;
	unsigned int i;

	if (new_size == old_size)
		return (ptr);
	if (ptr == NULL)
	{
		newspace = malloc(new_size);
		if (newspace == NULL)
			return (NULL);
		return (newspace);
	}
	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}
	newspace = malloc(new_size);
	if (newspace == NULL)
		return (NULL);
	for (i = 0; i < old_size && i < new_size; i++)
		newspace[i] = ((char *)ptr)[i];
	free(ptr);
	return (newspace);
}

/**
 * _memcpy - copies memory location in src to destination
 *
 *@dest: destination memory location
 *@src: source memory location
 *@n: number of bytes
 *Return: returns pointer to destination
 */
char *_memcpy(char *dest, char *src, unsigned int n)
{
	int i;

	i = 0;
	while (i < (int)n)
	{
		dest[i] = src[i];
		i++;
	}
	return (dest);
}

char *_memchunk(char *dest, char *src, unsigned int start, unsigned int end)
{
	unsigned int i, j;

	i = start;
	j = 0;
	while (j < end)
	{
		dest[j] = src[i];
		i++;
		j++;
	}
	return (dest);
}

char *parseDollar(char *buf, helper_t *helper)
{
	char *name;
	char *newbuf;
	char *value;
	env_t *envname;
	int i, j, k, start, size, end;
	env_t *env;
	int *total, *last;

	start = 0;
	env = helper->env;
	i = 0;
	total = helper->total;
	last = helper->last;
	end = _strlen(buf);
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
				newbuf = malloc(*total);
				_memcpy(newbuf, buf, start - 1);
				_memcpy(newbuf + start - 1, buf + (start + _strlen(name) + 1), *total - (_strlen(name) + _strlen(value)));
				free(buf);
				return (newbuf);
			}
			else
			{
				value = envname->value;
				size = *total + _strlen(value) + 1000;
				newbuf = malloc(size);
				if (start - 1 > 0)
				{
					_memcpy(newbuf, buf, start - 1);
					_memcpy(newbuf + (start - 1), value, _strlen(value) + 1);
				}
				else
					_memcpy(newbuf + (start - 1), value, _strlen(value) + 1);
				_memcpy(newbuf + (start - 1) + _strlen(value), buf + (start) + _strlen(name), *total);
				free(buf);
				buf = newbuf;
				*(helper->last) += (_strlen(value) - _strlen(name) - 1);
			}
			free(name);
		}
		if (start != 0)
		{
			i = start;
			start = 0;
		}
		i++;
		end = _strlen(buf);
	}
	return (buf);
}


/**
 * itoa - convert integer @n to string @s
 * @n: integer number
 * @s: character array
 *
 * Return: returns int for position of format
 */
int itoa(int n, char *s)
{
	int i = 0;

	if (n / 10 != 0)
		i = itoa(n / 10, s);
	else if (n < 0)
		s[i++] = '-';

	s[i++] = _abs(n % 10) + '0';
	s[i] = '\0';

	return (i);
}


/**
 * _abs - return absolute value of @num
 * @num: integer number
 *
 * Return: absolute value of @num
 */
int _abs(int num)
{
	if (num < 0)
		return (num * -1);
	else
		return (num);
}
