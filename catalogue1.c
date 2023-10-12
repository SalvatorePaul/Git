#include "myshell.h"
/**
 *get_node - Get the index of a node in a list_t linked list.
 *@head: Pointer to the head node of the list.
 *@node: Pointer to the node whose index is to be retrieved.
 *Return: The index of the node or -1 if the node is not found.
 */
ssize_t get_node(list_t *head, list_t *nde)
{
	size_t i = 0;

	while (head)
	{
		if (head == nde)
			return (i); /* Return the index of the node when found */
		head = head->next;
		i++;
	}
	return (-1);  /* Return -1 when the node is not found */
}
/**
 *list_to_strn - Converts a list of strings into an array of strings.
 *@head: Pointer to the first node of the list.
 *Return: An array of strings, or NULL if the conversion fails.
 */
char **list_to_strn(list_t *head)
{
	char **strn;
	list_t *node = head;
	char *str;
	size_t c = listLength(head), d;
	
	if (!head || !c)
		return (NULL); /* Return NULL if the list is empty or invalid */
	strn = malloc(sizeof(char *) * (c + 1));
	if (!strn)
		return (NULL); /* Return NULL if malloc fails */
	for (c = 0; node; node = node->next, c++)
	{
		str = malloc(lenstrn(node->str) + 1);
		if (!str)
		{
			for (d = 0; d < c; d++)
				free(strn[d]);
			free(strn);
			return (NULL); /* Return NULL if memory allocation for a string fails */
		}
		str = cpyStrn(str, node->str);
		strn[c] = str;
	}
	strn[c] = NULL; /* Set the last element of the array to NULL */
	return (strn); /* Return the array of strings */
}
/**
 *prints_list -  Prints all elements of a linked list of type list_t.
 *@h: Pointer to the first node of the list.
 *Return: The number of nodes in the list.
 */
size_t prints_list(const list_t *h)
{
	size_t count = 0;

	while (h)
	{
		/* Print the numeric value of the current node */
		prnt_strn(number_to_strn(h->num, 10, 0));
		/* Print a colon and a space */
		wrt_chr(':');
		wrt_chr(' ');
		/* Print the string value of the current node, or "(nil)" if NULL */
		prnt_strn(h->str ? h->str : "(nil)");
		/* Print a newline character */
		prnt_strn("\n");
		/* Move to the next node and increment the count */
		h = h->next;
		count++;
	}
	return (count);
}
/**
 * find_node - Finds a node in a linked list that matches a prefix and a character.
 *@node: Pointer to the head of the linked list.
 *@prefix: String to match as a prefix.
 *@c: The next character after the prefix to match.
 *Return: A pointer to the first matching node, or NULL if no match is found.
 */
list_t *find_node(list_t *node, char *prefix, char c)
{
	char *a = NULL;

	for (; node; node = node->next)
	{
	/* Check if the node's string starts with the given prefix */
	a = starts_with_strn(node->str, prefix);
	/* If p is not NULL and the next character matches or no character is specified */
	if (a && ((c == -1) || (*a == c)))
	return (node); /* Return the matching node */
	}

	return (NULL); /* No match found, return NULL */
}
/**
 *listLength - Calculate the length of a linked list.
 *@h: Pointer to the first node of the linked list.
 *Return: The number of nodes in the linked list.
 */
size_t listLength(const list_t *h)
{
	size_t a = 0;

	while (h)
	{
		/* Move to the next node and increment the count */
		h = h->next;
		a++;
	}
	return (a);
}
