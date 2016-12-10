#include <stdio.h>

char *splitstr(char *str, const char *delim, char **saveptr);

int main (void)
{
	char *token; /*this is where we will store tokens*/
	char cisfun[] = "c is fun"; /*string to split*/
	char pythonisnotfun[] = "python is not fun";
	char *save; /*save ptr for splitstr location*/
	char *pysave;
	char delim = ' '; /* need to specify a deliminator */

	token = splitstr(cisfun, &delim, &save); /* first call is as expected */
	printf("token: %s\n", token); /*outputs first token, c */

	token = splitstr(pythonisnotfun, &delim, &pysave); /* first call is as expected */
	printf("pytoken: %s\n", token); /*outputs first token, c */

	token = splitstr(NULL, &delim, &save); /* second call, do not specify a string*/
	printf("token: %s\n", token); /*returns the next token, is*/

	token = splitstr(NULL, &delim, &pysave);
	printf("pytoken: %s\n", token);

	token = splitstr(NULL, &delim, &save);
	printf("token: %s\n", token); /*returns the last token, fun*/

	token = splitstr(NULL, &delim, &pysave);
	printf("pytoken: %s\n", token);

	token = splitstr(NULL, &delim, &save);
	printf("token: %s\n", token); /*no more tokens, returns NULL*/

	token = splitstr(NULL, &delim, &pysave);
	printf("pytoken: %s\n", token);

	token = splitstr(NULL, &delim, &pysave);
	printf("pytoken: %s\n", token);

	printf("c is fun: %s\n", cisfun); /*original string is mangled w/ nulls and cannot be used!*/
}

/**
 * splitstr - splits a string into tokens by deliminator
 * first call requires a string, all subsequent calls
 * require NULL to get more tokens from saved string
 *
 * @str: string to tokenize
 * @delim: deliminator
 * @saveptr: ptr used to save locations
 */
char *splitstr(char *str, const char *delim, char **saveptr)
{
	char de;
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
	while (*str != '\0' && *str != *delim) /* move forward until we find a delim or null */
	{
		str++; /* we could check here for additional delims each time we rotate! */
		i++; /* for multiple delim setup (when we want to use ;, etc) */
	}
	if (*str == '\0') /* null found, this is our last word */
		*saveptr = NULL;
	else
		*saveptr = str + 1; /* save the letter after the null we just placed for next time */
	tok[i] = '\0';
	return (tok); /* return a pointer to our new token */
}
