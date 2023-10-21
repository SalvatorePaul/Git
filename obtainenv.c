#include "myshell.h"

/**
 * set_env - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Pointer to a structure containing potential arguments.
 *        Used to maintain a consistent function prototype.
 * @variable: The string representing the environment variable property.
 * @val: The string representing the environment variable value.
 * This function creates a new environment variable or modifies an existing one
 * with the specified property and value. If the variable already exists, it is
 * updated with the new value. If either 'var' or 'value' is NULL, no action is
 * taken.
 *
 *  Return: Always 0
 */
int set_env(info_t *info, char *variable, char *val)
{
	char *b; /*Pointer for checking if an existing variable matches  */
	char *buffer = NULL; /* Buffer to store the formatted environment variable */
	list_t *node; /* Linked list node for traversing environment variables */

	if (!variable || !val)
		return (0); /* If 'var' or 'value' is NULL, no action is taken. */
	buffer = malloc(lenstrn(variable) + lenstrn(val) + 2);
	if (!buffer)
		return (1); /* Allocation error, return 1 for failure. */
	cpyStrn(buffer, variable); /*Copy the 'var' string to the buffer. */
	catstrn(buffer, "="); /* Append '=' to the buffer. */
	catstrn(buffer, val); /* Append 'value' to the buffer. */
	node = info->env; /* Traverse the linked list of environment variables. */
	while (node)
	{
		b = starts_with_strn(node->str, variable);
		if (b && *b == '=')
		{
			free(node->str); /* Free the old value. */
			node->str = buffer; /* Update with the new value. */
			info->env_changed = 1; /* Indicate that the environment has changed. */
			return (0); /* Return 0 for success. */
		}
		node = node->next; /* Move to the next node in the list. */
	}
	/* If the variable doesn't exist, add it to the end of the list. */
	nodeadd_end(&(info->env), buffer, 0);
	free(buffer); /* Free the buffer since it's no longer needed. */
	info->env_changed = 1; /* Indicate that the environment has changed. */
	return (0); /* Return 0 for success. */
}
/**
 * unset_env - uninitialize an environment variable
 * @info: Pointer to a structure containing potential arguments.
 *        Used to maintain a consistent function prototype.
 * @var: The string representing the environment variable property to remove.
 * Return: 1 if the variable is deleted, 0 otherwise
 *
 */
int unset_env(info_t *info, char *var)
{
	char *p;
	size_t a = 0;
	list_t *node = info->env;

	/* Check if the environment list or the variable name is NULL */
	if (!node || !var)
		return (0);

	while (node)
	{
		/* Check if the variable starts with the specified name followed by '=' */
		p = starts_with_strn(node->str, var);
		if (p && *p == '=')
		{
			/* Delete the node containing the environment variable */
			info->env_changed = nodeDelete_at_index(&(info->env), a);
			/* Reset the index and start iterating from the beginning */
			a = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		a++;
	}
	return (info->env_changed); /* Return 1 if deleted, 0 otherwise */
}
/**
 * get_environ - Returns a string array copy of our environment variables.
 * @info: Pointer to a structure containing potential arguments.
 *        Used to maintain a consistent function prototype.
 * This function returns a copy of the environment variables as a string array.
 * If the environment variables have been modified, it updates the copy.
 * Return: A pointer to the string array representing the environment.
 */
char **get_environ(info_t *info)
{
	/* Check if the environment list is empty or has changed */
	if (!info->environ || info->env_changed)
	{
		/* Convert the environment list to a string array */
		info->environ = list_to_strn(info->env);
		/* Reset the environment change flag */
		info->env_changed = 0;
	}

	/* Return the updated or existing environment as a string array */
	return (info->environ);
}
