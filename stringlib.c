#include "shell.h"

/**
 * _strlen - returns the length of a given string
 *
 *
 * @s: string to check the length of
 * Return: returns the length of the string
 */
int _strlen(char *s)
{
	int i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

/**
 * _strcpy - copies an entire string to a buffer
 *
 *
 * @src: source string to copy
 * @dest: buffer to copy string into
 * Return: returns pointer to copied string
 */
char *_strcpy(char *dest, char *src)
{
	int i;
	char *retval;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = src[i];

	retval = dest;

	return (retval);
}

/**
 * _strdup - duplicates a string to a newly allocated space
 *
 * @str: string to duplicate
 * Return: returns pointer to duplicated string
 */
char *_strdup(char *str)
{
	char *dupe;

	if (str == NULL)
		return (NULL);
	dupe = malloc(_strlen(str) + 1);
	if (dupe == NULL)
		return (NULL);
	_strcpy(dupe, str);
	return (dupe);
}

/**
 * str_concat - concatenates two strings to a new string
 *
 *
 * @s1: first string
 * @s2: second string
 * Return: returns pointer to completed string
 */
char *str_concat(char *s1, char *s2)
{
	char *new;
	int i, j, size;

	if (s1 == NULL)
		s1 = "";
	if (s2 == NULL)
		s2 = "";
	size = (_strlen(s1) + _strlen(s2) + 1);
	new = malloc(size);
	if (new == NULL)
		return (NULL);
	_strcpy(new, s1);
	i = _strlen(s1);
	for (i = _strlen(s1), j = 0; i != size && s2[j] != '\0'; i++, j++)
		new[i] = s2[j];
	new[i] = '\0';
	return (new);
}

/**
 * _strcmp - compares two strings, NOT null terminator!
 *
 *
 * @s1: first string
 * @s2: second string
 * Return: difference in ASCII value between string 1 and string 2
 */
int _strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	if (s1[i] == '\0' || s2[i] == '\0')
		return (-1);
	while (s1[i] != '\0' && s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return ((s1[i] - '0') - (s2[i] - '0'));
		i++;
	}
	return (0);
}

/**
 * _memset - fills memory with constant bytes
 *
 * @s: location to fill
 * @b: char to fill location with
 * @n: number of bytes to fill
 * Return: returns pointer to location filled
 */

char *_memset(char *s, char b, unsigned int n)
{
	int i;

	i = 0;
	while (n > 0)
	{
		s[i] = b;
		i++;
		n--;
	}
	return (s);
}

/**
 * _strcat - concat strings, src appended to dest
 *
 * @dest: destination string
 * @src: source string
 * Return: returns finished string
 */
char *_strcat(char *dest, char *src)
{
	int i, j;

	i = j = 0;
	while (dest[i] != '\0')
		i++;
	while (src[j] != '\0')
		dest[i++] = src[j++];
	dest[i] = '\0';
	return (dest);
}
