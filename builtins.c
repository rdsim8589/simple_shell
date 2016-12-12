#include "shell.h"

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
		chdir(home);
		setEnvPtr("PWD", home);
		setEnvPtr("OLDPWD", oldcwd);
		return (1);
	}
	if (tok[0] == '-')
	{
		getenv = getEnvPtr("OLDPWD");
		if (getenv == NULL)
		{
			_putstring("cd: OLDPWD not set"); return (0);
		}
		else
		{
			tok = getEnvPtr("OLDPWD") + 7;
			_putstring(tok);
		}
	}
	if (chdir(tok) != -1)
	{
		setEnvPtr("PWD", tok);
		setEnvPtr("OLDPWD", oldcwd);
		return (1);
	}
	_putstring("cd: Invalid folder."); /* Need error codes for why it's invalid!
					    *  Need to know if it's an access issue,
					    * or the folder simply doesn't exist.
					   */
	return (0);
}

int checkBuiltins(char *inp, char *save)
{
	int i;
	char *value, *tok;
	char delim = ' ';

	if (allstrcmp(inp, "env") == 0)
	{
		i = 0;
		while (environ[i] != NULL)
		{
			_putstring(environ[i]);
			_putchar(10);
			i++;
		}
		return (1);
	}
	else if (allstrcmp(inp, "exit") == 0)
	{
		tok = splitstr(NULL, &delim, &save);
		if (tok != NULL)
			i = atoi(tok);
		else
			i = 0;
		_exit(i);
	}
	else if (allstrcmp(inp, "setenv") == 0)
	{
		tok = splitstr(NULL, &delim, &save);
		value = splitstr(NULL, &delim, &save);
		if (tok == NULL)
		{
			_putstring("Bad env name.");
			_putchar(10);
			return (0);
		}
		_putstring(tok);
		_putchar(10);
		if (value == NULL)
		{
			_putstring("Bad value.");
			_putchar(10);
			return (0);
		}
		setEnvPtr(tok, value);
	}
	else if (allstrcmp(inp, "unsetenv") == 0)
		_putstring("Run unsetenv");
	else if (allstrcmp(inp, "history") == 0)
		_putstring("Run history");
	else if (allstrcmp(inp, "cd") == 0)
		cdBuiltin(save);
	else if (allstrcmp(inp, "help") == 0)
		_putstring("Run help");
	else if (allstrcmp(inp, "exit") == 0)
		_putstring("Run exit");
	else
		return (0);
	return (1);
}

char *getEnvPtr(char *name)
{
	int i;

	i = 0;
	while (environ[i] != NULL)
	{
		if (_strcmp(name, environ[i]) == 0)
			return (environ[i]);
		i++;
	}
	return (NULL);
}

char *setEnvPtr(char *envname, char *value)
{
	char *envPtr, *newEnvPtr;
	char equals = '=';
	int i = 0;

	while (environ[i] != NULL)
	{
		if (_strcmp(envname, environ[i]) == 0)
			break;
		i++;
	}
	if (environ[i] == NULL)
	{
		environ[i + 1] = NULL;
		envPtr = str_concat(envname, &equals);
		_putstring(envPtr);
	}
	else
	{
		envPtr = _strdup(environ[i]);
	}
	envPtr[_strlen(envname) + 1]  = '\0';
	newEnvPtr = str_concat(envPtr, value);
	environ[i] = newEnvPtr;
	_putstring(environ[i]);
	return (environ[i]);
}
