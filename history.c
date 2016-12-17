#include "shell.h"
/**
 * add_hist - adds the user entry to hist linked list
 * @total: size of the num of bytes to copy
n * @head: a pointer to the head of hist linked list
 * @buf: a pointer to the string of chars to be copied
 */

hist_t *add_hist(int total, hist_t **hist_head, char *buf)
{
	hist_t *new_hist, *current;
	int i;

	if (buf[0] == ' ')
	{
		return (NULL);
	}
	new_hist = malloc(sizeof(hist_t));
	if (new_hist == NULL)
		return (NULL);
	new_hist->entry = malloc(sizeof(char) * total);
	_memcpy(new_hist->entry, buf, total);

	for (i = 0; i <= total - 1; i++)
	{
		if ((new_hist->entry)[i] == '\n')
			(new_hist->entry)[i] = '\0';
	}
	new_hist->next = NULL;

	if (*hist_head == NULL)
	{
		*hist_head = new_hist;
	}
	else
	{
		current = *hist_head;
		while (current->next != NULL)
			current = current->next;
		current->next = new_hist;
	}
	return (*hist_head);
}
/**
 * clear_hist - clear the history linked list
 * @hist_head: a pointer ot the hist_head of the hist linked list
 */
void clear_hist(hist_t **hist_head)
{
	hist_t *hold;

	while (*hist_head != NULL)
	{
		hold = *hist_head;
		(*hist_head) = (*hist_head)->next;
		free(hold->entry);
		free(hold);
	}
}
/**
 * print_hist - prints out the entry of the hist linked list
 * @hist_head: the hist_hist_head of the hist linked list
 */
void print_hist(hist_t *hist_head)
{
	int i;

	i = 0;
	while (hist_head != NULL)
	{
		_putchar(i + '0');
		_putchar(' ');
		_putstring(hist_head->entry);
		_putchar('\n');
		hist_head = hist_head->next;
		i++;
	}
}
/**
 * push_hist - push the historyfrom the current sessions into HIST_FILE
 * @hist_head: the head to the hist link list
 * @head: the head to the environment linked list
 */
void push_hist(hist_t *hist_head, env_t *head)
{
	mode_t mode;
	int file, err_w, err_c;
	env_t *env_var;
	char *home;

	if (!hist_head || !head)
		exit(100);
	/* get the env for $HOME */
	env_var = getEnvPtr("HOME", head);
	home = env_var->value;
	home = dir_concat(home, HIST_FILE);
	mode = S_IRUSR | S_IWUSR;
	file = open(home, O_CREAT | O_WRONLY | O_TRUNC, mode);
	if (file == -1)
	{
		perror("Error opening file: ");
		free(home);
		exit(101);
	}

	while (hist_head != NULL)
	{
		if (hist_head->entry)
		{
			err_w = write(file, hist_head->entry, _strlen(hist_head->entry));
			if (err_w == -1 || err_w != _strlen(hist_head->entry))
			{
				free(home);
				exit(102);
			}
			err_w = write(file, "\n", 1);
			if (err_w == -1 || err_w != 1)
			{
				free(home);
				exit(103);
			}
		}
		hist_head = hist_head->next;
	}
	err_c = close(file);
	if (err_c == -1)
	{
		free(home);
		exit(104);
	}
}
/**
 * pull_hist - pulls entries from history and added it to hist linked list.
 * the linked list will only store up to 4096 lines
 * @hist_head: pointer to the head of the hist linked list
 * @head: the head of the enviroment linked list
 */

hist_t *pull_hist(hist_t **hist_head, env_t *head)
{
	char *home, *hist_line, delim, *saveptr, *buf;
	env_t *env_var;
	hist_t *current_node;
	int file, err_c, err_r, buf_len, i;
	struct stat st;

	/* get the home and concat with the history direct */
	env_var = getEnvPtr("HOME", head);
	home = env_var->value;
	home = dir_concat(home, HIST_FILE);

	/* read the history file into a big buffer */
	file = open(home, O_RDONLY);
	if (file == -1)
	{
		_putstring("unable to open hist file");
		exit(300);
	}
	/* using stat struct to get file size */
	if (stat(home, &st) == 0)
	{
		buf_len = st.st_size;
	}
	else
	{
		_putstring("failed to get count of hist_file");
		exit(301);
	}
	buf = malloc(sizeof(char) * buf_len);
	err_r = read(file, buf, buf_len);
	if (err_r == -1)
	{
		_putstring("unable to read hist_file");
	}
	buf[buf_len-1] = '\0';
	delim = '\n';
	i = 1;
	hist_line = splitstr(buf, &delim, &saveptr);
	while (hist_line != NULL)
	{
		add_hist(_strlen(hist_line) + 1, hist_head, hist_line);
		/* if the counter is greater than 4096 remove the head node of history  */
		if (i > 4096)
		{
			current_node = *hist_head;
			*hist_head = (*hist_head)->next;
			free(current_node->entry);
			free(current_node);
		}
		/* add to the end of the history of the linked list */
		hist_line = splitstr(NULL, &delim, &saveptr);
		i++;
	}
	print_hist(*hist_head);
	err_c = close(file);
	if (err_c == -1)
	{
		_putstring("fail to close hist_file");
		exit(302);
	}
	/* free buf */
	free(buf);
	free(home);
	return (*hist_head);
}
