#include "shell.h"
/**
 * add_hist - adds the user entry to hist linked list
 * @total: size of the num of bytes to copy
 * @head: a pointer to the head of hist linked list
 * @buf: a pointer to the string of chars to be copied
 */

hist_t *add_hist(int total, hist_t **hist_head, char *buf)
{
	hist_t *new_hist;
	hist_t *current;

	if (buf[0] == ' ')
	{
		return (NULL);
	}
	new_hist = malloc(sizeof(hist_t));
	if (new_hist == NULL)
		return (NULL);
	new_hist->entry = malloc(sizeof(char) * total);
	_memcpy(new_hist->entry, buf, total);
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
