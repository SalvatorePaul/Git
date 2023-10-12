#include "myshell.h"

/**
 * unset_alias - Unset an alias by its name.
 * @info: A parameter structure containing context and data.
 * @str: The name of the alias to unset.
 * This function unsets an alias specified by its name 'str'. It first checks if
 * the alias contains a '=' character to ensure it's a valid alias format. If
 * successful, it removes the alias from the alias list maintained in 'info'.
 *
 * Return: Always 0 on success, 1 on error.
 */
int unset_alias(info_t *info, char *str)
{
	char c;
	char *p;
	int rev;
	
	/* Check if the alias contains a '=' character. */
	p = str_chr(str, '=');
	if (!p)
		return (1);
		/* Temporarily replace '=' with a null terminator. */
	c = *p;
	*p = 0;
	/* Attempt to delete the alias from the alias list. */
	rev = nodeDelete_at_index(&(info->alias),
		get_node(info->alias, find_node(info->alias, str, -1)));
	/* Restore the '=' character. */
	*p = c;
	/* Return 0 on success, 1 on error. */
	return (rev);
}

/**
 * set_alias - sets alias to given string.
 * @info: A structure containing context and data.
 * @str: The string representing the alias in the format "alias=value".
 * This function sets an alias specified by the string 'str' to a given value.
 * It first checks if 'str' is in the valid alias format (contains '=').
 * If 'str' is a valid alias and successfully set, the function returns 0 to
 * indicate success. If 'str' is not in the correct format or if there is an
 * error during alias setting, the function returns 1.
 *
 * Return: Always 0 on success, 1 on error.
 */
int set_alias(info_t *info, char *str)
{
	char *p;
	
	/* Check if 'str' contains '=' to ensure valid alias format. */
	p = str_chr(str, '=');
	/* Check if 'str' contains a valid value part (not empty). */
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(info, str));
		
	/* Unset the alias if it already exists. */
	unset_alias(info, str);
	/* Add the alias to the end of the alias list. */
	return (nodeadd_end(&(info->alias), str, 0) == NULL);
}

/**
 * hist_cmd - Display the command history list.
 * @info: A structure containing potential arguments and context.
 * This function displays the command history list with line numbers
 * preceding each command, starting at 0. It utilizes the information
 * stored in the provided 'info' structure to access the command history.
 *  Return: Always 0 to indicate success.
 */
int hist_cmd(info_t *info)
{
	prints_list(info->history);
	return (0);
}

/**
 * print_alias - Print the contents of an alias.
 * @node: The alias node containing the alias string.
 * This function prints the contents of an alias, including both the alias name
 * and its corresponding value. It expects a valid 'node' containing the alias
 * string in the format "alias=value".
 *
 * Return: Always 0 on success, 1 on error.
 */
int print_alias(list_t *node)
{
	char *p = NULL;
	char *a = NULL;

	if (node)
	{
		p = str_chr(node->str, '=');
		/* Print the alias name enclosed in single quotes. */
		for (a = node->str; a <= p; a++)
			wrt_chr(*a);
		wrt_chr('\'');
		/* Print the alias value. */
		prnt_strn(p + 1);
		prnt_strn("'\n");
		return (0);  /* Success */
	}
	return (1); /* Error: Invalid node */
}

/**
 * alias_cmd - Manage shell aliases, mimicking the alias builtin (man alias).
 * @info: A structure containing potential arguments and context.
 * This function provides functionality similar to the 'alias' built-in command.
 * When invoked without arguments, it prints all defined aliases and their
 * corresponding values. When given arguments in the format "alias=value," it
 * sets new aliases or prints existing aliases matching the provided names.
 *  Return: Always 0.
 */
int alias_cmd(info_t *info)
{
	list_t *node = NULL;
	int a = 0;
	char *p = NULL;

	if (info->argc == 1)
	{
		/* Print all defined aliases and their values. */
		node = info->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (a = 1; info->argv[a]; a++)
	{
		p = str_chr(info->argv[a], '=');
		if (p)
			set_alias(info, info->argv[a]); /* Set new alias. */
		else
			print_alias(find_node(info->alias, info->argv[a], '=')); /* Print existing alias. */
	}

	return (0); /* Success. */
}
