#include "myshell.h"
/**
 *print_strList - Prints the string elements of a list_t linked list.
 *@h: Pointer to the first node of the list.
 *Return: The size of the list.
 */
size_t print_strList(const list_t *h)
{
	size_t i = 0;

	/* Iterate through the linked list */
	for (; h; h = h->next)
	{
		/* Print the `str` element or "(nil)" if NULL */
		prnt_strn(h->str ? h->str : "(nil)");

		prnt_strn("\n");
		i++; /* Increment the counter */
	}
	return (i);
}
/**
 *freeList - Frees all nodes and their string elements in a list.
 *@head_pter: Address of a pointer to the head node of the list.
 *Return: void
 */
void freeList(list_t **head_pter)
{
	list_t *node;
	list_t *next_node;
	list_t *head;

	if (!head_pter || !*head_pter)
		return; /* Return early if the list is empty or invalid */
	head = *head_pter;
	node = head;
	while (node)
	{
		next_node = node->next;
		free(node->str); /* Free the string element of the current node */
		free(node); /* Free the current node */
		node = next_node; /* Move to the next node */
	}
	*head_pter = NULL; /* Set head pointer to NULL to indicate an empty list */
}
/**
 *nodeadd_end - Adds a new node at the end of a list_t linked list.
 *@head: A pointer to a pointer to the head node of the list.
 *@strn: The string to be stored in the new node.
 *@numb: The node index used by history.
 *Return: The address of the new node, or NULL if it fails.
 */
list_t *nodeadd_end(list_t **head, const char *strn, int numb)
{
	list_t *new_node;
	list_t *node;

	if (!head)
		return (NULL); /* Return NULL if invalid arguments are provided */
	node = *head;
	new_node = malloc(sizeof(list_t)); /* Allocate memory for the new node */
	if (!new_node)
		return (NULL); /* Return NULL if malloc fails */
	mem_set((void *)new_node, 0, sizeof(list_t));
	new_node->num = numb;
	if (strn)
	{
		new_node->str = dupStrn(strn); /* Copy the string into the new node */
		if (!new_node->str)
		{
			free(new_node);
			return (NULL); /* Return NULL if strdup fails */
		}
	}
	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = new_node;
	}
	else
		*head = new_node;
	return (new_node); /* Return the address of the new node */
}
/**
 *nodeDelete_at_index- Deletes a node at the given index in a list_t,
 *@head: Address of a pointer to the first node.
 *@index: Index of the node to delete.
 *Return: 1 on success, 0 on failure.
 */
int nodeDelete_at_index(list_t **head, unsigned int index)
{
	list_t *node;
	list_t *prev_node;
	unsigned int b = 0;

	/* Check if the head pointer or the list is NULL */
	if (!head || !*head)
	return (0);

	switch (index)
	{
	case 0:
	/* Special case for deleting the head node */
	node = *head;
	*head = (*head)->next;
	free(node->str);
	free(node);
	return (1);
	default:
	/* Regular case for deleting a node at a specific index */
	node = *head;
	for (b = 0; node; b++)
	{
	if (b == index)
	{
	prev_node->next = node->next;
	free(node->str);
	free(node);
	return (1);
	}
	prev_node = node;
	node = node->next;
	}
	return (0);
	}
}
/**
 *nodeadd - Adds a new node to the start of a list_t linked list.
 *@head: Address of a pointer to the head node.
 *@strn: The string to be stored in the new node.
 *@numb: The node index used by history.
 *Return: The address of the new node, or NULL if it fails.
 */
list_t *nodeadd(list_t **head, const char *strn, int numb)
{
	list_t *new_head;

	if (!head)
		return (NULL); /* Return NULL if invalid arguments are provided */
	new_head = malloc(sizeof(list_t)); /* Allocate memory for the new node */
	if (!new_head)
		return (NULL); /* Return NULL if malloc fails */
	mem_set((void *)new_head, 0, sizeof(list_t));
	new_head->num = numb;
	if (strn)
	{
		new_head->str = dupStrn(strn); /* Copy the string into the new node */
		if (!new_head->str)
		{
			free(new_head);
			return (NULL); /* Return NULL if strdup fails */
		}
	}
	new_head->next = *head;
	*head = new_head; /* Update the head pointer to the new node */
	return (new_head); /* Return the address of the new node */
}
