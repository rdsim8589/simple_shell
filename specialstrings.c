#include "shell.h"

/**
 * splitstr - splits a string into tokens by deliminator
 * first call requires a string, all subsequent calls
 * require NULL to get more tokens from saved string
 *
 * @str: string to tokenize
 * @delim: deliminator
 * @saveptr: ptr used to save locations
 * Return: returns a pointer to our new token
 */
char *splitstr(char *str, const char *delim, char **saveptr)
{
	char *tok;
	int i;

	if (str == NULL) /*if str is null, we check saveptr*/
	{
		if (*saveptr == NULL) /*if saveptr is also null, we're done*/
			return (NULL);
		str = *saveptr; /*if saveptr isn't null, let's drag it back up */
	}
	i = 0;
	tok = str; /* tok will start where str starts now */
	while (*str != '\0' && *str != EOF && *str != *delim)
	{
		str++; /* we could check here for additional delims each time we rotate! */
		i++; /* for multiple delim setup (when we want to use ;, etc) */
	}
	if (*str == '\0') /* null found, this is our last word */
		*saveptr = NULL;
	else
		if (saveptr != NULL)
			*saveptr = str + 1; /* save the letter after the null */
	tok[i] = '\0';
	return (tok); /* return a pointer to our new token */
}

/**
 * allstrcmp - compares two strings, INCLUDING null terminator!
 *
 *
 * @s1: first string
 * @s2: second string
 * Return: difference in ASCII value between string 1 and string 2
 */
int allstrcmp(char *s1, char *s2)
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
	if (s1[i] == '\0' && s2[i] == '\0')
		return (0);
	return (i);
}

/**
 * dir_concat - concatenates two strings to a new string, appends a / between
 *
 *
 * @s1: first string
 * @s2: second string
 * Return: returns pointer to completed string
 */
char *dir_concat(char *s1, char *s2)
{
	char *new;
	int i, j, size;

	if (s1 == NULL)
		s1 = "";
	if (s2 == NULL)
		s2 = "";
	size = (_strlen(s1) + _strlen(s2) + 3);
	new = malloc(size);
	if (new == NULL)
		return (NULL);
	_strcpy(new, s1);
	i = _strlen(s1);
	new[i] = '/';
	for (i = (_strlen(s1) + 1), j = 0; i != size && s2[j] != '\0'; i++, j++)
		new[i] = s2[j];
	new[i] = '\0';
	return (new);
}

/**
 * _atoi - Converts a character array to an integer
 *
 * Description: Respects leading '-' and '+' signs
 * integer inside string may be preceded by any number of non-integer values
 * but will only copy the first whole integer found.
 *
 * @s: character array to convert
 * Return: returns integer from array, returns 0 if none found
 */
int _atoi(char *s)
{
	int retval, i, neg;

	i = 0;
	retval = 0;
	neg = -1;
	while (s[i] != '\0' && (s[i] < '0' || s[i] > '9'))
	{
		if (s[i] == '-')
			neg *= -1;
		i++;
	}
	while (s[i] != '\0' && (s[i] >= '0' && s[i] <= '9'))
		retval = (retval * 10) - (s[i++] - '0');
	return (retval * neg);
}
