#include "myshell.h"

/**
 * get_env -  Get the value of an environment variable
 * @info: Pointer to a structure containing potential arguments,
 *              used to maintain consistency in function prototypes.
 * @name: Name of the environment variable to retrieve.
 * This function is responsible for retrieving the value of an environment
 * variable specified by the 'name' parameter from the environment variables
 * stored in the 'info' structure.
 *
 * Return: Pointer to the value of the environment variable if found,
 *         or NULL if not found.
 */
char *get_env(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *c;

	while (node)
	{
		/* Check for variable match */
		c = starts_with_strn(node->str, name);
		if (c && *c)
			return (c);
		node = node->next;
	}
	return (NULL); /* Environment variable not found */
}

/**
 * env_cmd - prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int env_cmd(info_t *info)
{
	/* Call the 'print_list_str' function to print the environment variables */
	print_strList(info->env);
	return (0); /* Indicate successful execution */
}

/**
 * setenv_cmd - Initialize a new environment variable or modify an existing one
 * @info: A pointer to a structure containing potential arguments.
 *              Used to maintain a consistent function prototype.
 * This function is responsible for setting or modifying environment variables.
 * It takes a structure 'info' as an argument, which contains the necessary information.
 *  Return: Always 0 on success; otherwise, returns 1.
 */
int setenv_cmd(info_t *info)
{
	/* Check if the number of arguments is correct */
	if (info->argc != 3)
	{
		 _errputs("Wrongs number of arguements\n");
		return (1);
	}
	/* Call setenv_cmd function to set or modify the environment variable*/
	if (_mysetenv(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}
/**
 * unsetenv_cmd - uninitialize an environment variable
 * @info: Pointer to a structure containing potential arguments,
 *              used to maintain a consistent function prototype.
 * This function is responsible for removing one or more environment variables
 * specified as command-line arguments in the 'info' structure.
 *  Return: Always 0
 */
int unsetenv_cmd(info_t *info)
{
	int c;
	
	/* Check if too few arguments were provided */
	if (info->argc == 1)
	{
		 _errputs("More arguements needed.\n");
		return (1);
	}
	/* Loop through the arguments and call _unsetenv to remove each environment variable */
	for (c = 1; c <= info->argc; c++)
		_myunsetenv(info, info->argv[c]);

	return (0);
}

/**
 * populateEnv_list - Populate the environment linked list
 * @info: Pointer to a structure containing potential arguments,
 *              used to maintain a consistent function prototype.
 * Return: Always 0 to indicate successful completion.
 */
int populateEnv_list(info_t *info)
{
	size_t c;
	list_t *node = NULL; /* Initialize a linked list node */
	
	/* Loop through the 'environ' array to add environment variables to the linked list */
	for (c = 0; environ[c]; c++)
		nodeadd_end(&node, environ[c], 0);
	info->env = node; /* Assign the created linked list to the 'env' member of the 'info' structure */
	return (0); /* Indicate successful completion */
}
