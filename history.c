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

	while (hist_head != NULL)
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
		_putstring(hist_head->entry);
		_putchar('\n');
		hist_head = hist_head->next;
		i++;
	}
}
