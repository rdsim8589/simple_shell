#include "shell.h"

/**
 * cdBuiltin - function to execute the builtin CD command
 * sets the pwd and oldpwd environmental variables appropriately
 *
 * @save: saveptr for tokens
 * Return: 1 on success, 0 on failure
 */
int cdBuiltin(char *save)
{
	char *oldcwd, *home, *tok, *getenv;
	char delim = ' ';

	tok = NULL;
	oldcwd = getcwd(NULL, 100);
	home = getEnvPtr("HOME");
	home = home + 5;

	tok = splitstr(NULL, &delim, &save);
	if (tok == NULL) /*If no argument, we want to go HOME */
	{
		chdir(home); /*Change to the home directory, we need to error check this!*/
		setEnvPtr("PWD", home); /*sets PWD to home, since we're going there, and OLDPWD to where we're at now*/
		setEnvPtr("OLDPWD", oldcwd); /*this function should never fail, so probably don't need to error check*/
		return (1); /*return 1, success*/
	}
	if (tok[0] == '-') /*if we do cd -*/
	{
		if (getEnvPtr("OLDPWD") == NULL)
		{
			_putstring("cd: OLDPWD not set"); /*if we don't have an OLDPWD, can't go there*/
			return (0);
		}
		tok = getEnvPtr("OLDPWD") + 7; /*if we do, advance the ptr by 7 to get past OLDPWD=*/
	}
	if (chdir(tok) != -1) /*can we go there? this includes OLDPWD*/
	{
		setEnvPtr("PWD", tok); /*if so, set our PWD and OLDPWD and bounce*/
		setEnvPtr("OLDPWD", oldcwd);
		return (1);
	}
	_putstring("cd: Invalid folder."); /* Need error codes for why it's invalid!
					    *  Need to know if it's an access issue,
					    * or the folder simply doesn't exist.
					    */
	return (0);
}

/**
 * checkBuiltins - checks for builtin commands matching the first inputted word
 *
 * @inp: input string from main
 * @save: saveptr for tokens
 *
 * Return: returns 1 on success, 0 on failure.
 */
int checkBuiltins(char *inp, char *save)
{
	int i;
	char *value, *tok;
	char delim = ' ';

	if (allstrcmp(inp, "env") == 0) /*if the first word is env, run env*/
	{
		i = 0;
		while (environ[i] != NULL)
		{
			_putstring(environ[i]); /*print the whole environ list*/
			_putchar(10);
			i++;
		}
	}
	else if (allstrcmp(inp, "exit") == 0) /* probably split this into a dif func*/
	{
		tok = splitstr(NULL, &delim, &save);
		if (tok != NULL)
			i = atoi(tok); /*atoi if there's an arg so we can exit w/ different statuses*/
		else
			i = 0;
		_exit(i);
	}
	else if (allstrcmp(inp, "setenv") == 0) /*setenv*/
	{
		tok = splitstr(NULL, &delim, &save);
		value = splitstr(NULL, &delim, &save);
		if (setEnvPtr(tok, value) == NULL) /*return 0 on failure, otherwise will fall through to 1*/
			return (0);
	}
	else if (allstrcmp(inp, "unsetenv") == 0)
		_putstring("Run unsetenv");
	else if (allstrcmp(inp, "history") == 0)
		_putstring("Run history");
	else if (allstrcmp(inp, "cd") == 0)
		cdBuiltin(save);
	else if (allstrcmp(inp, "help") == 0)
		_putstring("Run help");
	else
		return (0);

	return (1);
}


/**
 * getEnvPtr - gets a pointer to a matching environment variable
 *
 * @name: name of environment variable to search for
 * Return: returns a pointer to the variable, or NULL if none found
 */
char *getEnvPtr(char *name)
{
	int i;

	i = 0;
	while (environ[i] != NULL)
	{
		if (_strcmp(name, environ[i]) == 0) /*just scrapes the env list, and if it finds
						     *a partial match, it returns it, including the name*/
			/*Does it even need to return the name, or should we lop it off here and save time?*/
			return (environ[i]);
		i++;
	}
	return (NULL);
}


/**
 * setEnvPtr - sets an environment variable to a new value
 * creates a new one if none found
 *
 * @envname: Name to variable to set/create
 * @vale: Value for environmental varialbe
 * Return: returns a pointer to the new environmental variable
 * need to error check the malloc'd returns and return null on fail
 */
char *setEnvPtr(char *envname, char *value)
{
	char *envPtr, *newEnvPtr;
	char equals = '=';
	int i = 0;
/*this needs error checking on the return from malloc'd functions*/
	while (environ[i] != NULL)
	{
		if (_strcmp(envname, environ[i]) == 0) /* loop til you find a match, or null*/
			break;
		i++;
	}
	if (environ[i] == NULL) /*if we're at null*/
	{
		environ[i + 1] = NULL; /*add null one down, so functions will still stop there*/
		envPtr = str_concat(envname, &equals); /*concat an = sign to envname, prob easier way than this*/
	}
	else
	{
		envPtr = _strdup(environ[i]); /*if we're not at null, we found one, so just copy it*/
	}
	envPtr[_strlen(envname) + 1]  = '\0'; /*make sure there's a null after NAME=*/
	newEnvPtr = str_concat(envPtr, value); /*concat on the value*/
	environ[i] = newEnvPtr; /*pop it into environ*/
	return (environ[i]); /*return the result*/
}
