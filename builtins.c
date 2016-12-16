#include "shell.h"

/**
 * cdBuiltin - function to execute the builtin CD command
 * sets the pwd and oldpwd environmental variables appropriately
 *
 * @save: saveptr for tokens
 * Return: 1 on success, 0 on failure
 */
int cdBuiltin(char *save, env_t *head)
{
	char *oldcwd, *home, *tok;
	char delim = ' ';

	tok = NULL;
	oldcwd = getcwd(NULL, 100);
	home = (getEnvPtr("HOME", head))->value;

	tok = splitstr(NULL, &delim, &save);
	if (tok == NULL) /*If no argument, we want to go HOME */
	{
		chdir(home); /*Change to the home directory, we need to error check this!*/
		setEnvPtr("PWD", home, head); /*sets PWD to home, since we're going there, and OLDPWD to where we're at now*/
		setEnvPtr("OLDPWD", oldcwd, head); /*this function should never fail, so probably don't need to error check*/
		free(oldcwd);
		return (1); /*return 1, success*/
	}
	if (tok[0] == '-') /*if we do cd -*/
	{
		if (getEnvPtr("OLDPWD", head) == NULL)
		{
			_putstring("cd: OLDPWD not set"); /*if we don't have an OLDPWD, can't go there*/
			free(oldcwd);
			return (0);
		}
		tok = (getEnvPtr("OLDPWD", head))->value; /*if we do, advance the ptr by 7 to get past OLDPWD=*/
	}
	if (chdir(tok) != -1) /*can we go there? this includes OLDPWD*/
	{
		setEnvPtr("PWD", tok, head); /*if so, set our PWD and OLDPWD and bounce*/
		setEnvPtr("OLDPWD", oldcwd, head);
		free(oldcwd);
		return (1);
	}
	_putstring("cd: Invalid folder."); /* Need error codes for why it's invalid!
					    *  Need to know if it's an access issue,
					    * or the folder simply doesn't exist.
					    */
	free(oldcwd);
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
int checkBuiltins(char *inp, char *save, env_t **environ, helper_t *helper)
{
	char *value, *tok;
	char delim = ' ';
	hist_t *hist_head;

	hist_head = helper->hist_head;
	if (allstrcmp(inp, "env") == 0) /*if the first word is env, run env*/
	{
		listEnv(environ);
	}
	else if (allstrcmp(inp, "exit") == 0) /* probably split this into a dif func*/
	{
		tok = splitstr(NULL, &delim, &save);
		exitBuiltin(tok, inp, environ, helper);
	}
	else if (allstrcmp(inp, "setenv") == 0) /*setenv*/
	{
		tok = splitstr(NULL, &delim, &save);
		value = splitstr(NULL, &delim, &save);
		setEnvPtr(tok, value, *environ);
	}
	else if (allstrcmp(inp, "unsetenv") == 0)
	{
		tok = splitstr(NULL, &delim, &save);
		unsetEnv(tok, environ);
	}
	else if (allstrcmp(inp, "history") == 0)
		print_hist(hist_head);
	else if (allstrcmp(inp, "cd") == 0)
		cdBuiltin(save, *environ);
	else if (allstrcmp(inp, "help") == 0)
		_putstring("Run help");
	else
		return (0);

	return (1);
}

void exitBuiltin(char *tok, char *inp, env_t **environ, helper_t *helper)
{
	int i;

	free(inp);
	clear_hist(&(helper->hist_head));
	free_list(*environ);
	free(helper->printed);
	free(helper->total);
	free(helper->last);
	free(helper->bufsize);
	free(helper);
	if (tok != NULL)
		i = atoi(tok); /*atoi if there's an arg so we can exit w/ different statuses*/
	else
		i = 0;
	_exit(i);
}

int listEnv(env_t **environ)
{
	env_t *walk;

	walk = *environ;
	if (walk == NULL)
	{
		_putstring("Issue printing environment variables!");
		return (-1);
	}
	while (walk != NULL)
	{
		_putstring(walk->name);
		_putchar('=');
		_putstring(walk->value);
		_putchar('\n');
		walk = walk->next;
	}
	return (1);
}

/**
 * unsetEnv - removes an environmental variable
 *
 * @name: name of environmental variable to remove
 * Return: returns 1 on success, -1 on failure
 */
int unsetEnv(char *name, env_t **head)
{
	int i;
	env_t *temp, *last;

	temp = *head;
	if (name == NULL)
	{
		_putstring("Invalid name.\n");
		return (-1);
	}
	i = 0;
	while (temp != NULL)
	{
		if (_strcmp(name, temp->name) == 0)
		{
			if (i == 0)
			{
				*head = temp->next;
				free(temp->name);
				free(temp->value);
				free(temp);
			}
			else
			{
				last->next = temp->next;
				free(temp->name);
				free(temp->value);
				free(temp);
			}
			return (1);
		}
		last = temp;
		temp = temp->next;
		i++;
	}
	_putstring("No such environmental variable.");
	return (-1);
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
env_t *setEnvPtr(char *envname, char *value, env_t *head)
{
	env_t *environ;
	char *newvalue;

	environ = head;
	if (envname == NULL)
	{
		_putstring("Invalid environment variable name.\n");
		return (NULL);
	}
	if (value == NULL)
	{
		_putstring("Invalid value for enviroment variable.\n");
		return (NULL);
	}
	environ = getEnvPtr(envname, head);
	if (environ != NULL)
	{
		newvalue = _strdup(value);
		free(environ->value);
		environ->value = newvalue;
		return (environ);
	}
	return (addEnv(&head, envname, value)); /*return the result*/
}
