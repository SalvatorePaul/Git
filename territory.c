#include "myshell.h"

/**
 * get_env - Get the value of an environment variable.
 *
 * @info: Pointer to a structure containing potential arguments,
 * used to maintain consistency in function prototypes.
 * @name: Name of the environment variable to retrieve.
 *
 * This function is responsible for retrieving the value of an environment
 * variable specified by the 'name' parameter from the environment variables
 * stored in the 'info' structure.
 *
 * Return: Pointer to the value of the environment variable if found,
 * or NULL if not found.
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
 * env_cmd - Prints the current environment.
 *
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 *
 * Return: Always 0.
 */
int env_cmd(info_t *info)
{
	/* Call the 'print_list_str' function to print the environment variables */
	print_strList(info->env);
	return (0); /* Indicate successful execution */
}

/**
 * setenv_cmd - Initialize a new environment variable or modify an existing one
 *
 * @info: A pointer to a structure containing potential arguments.
 * Used to maintain a consistent function prototype.
 *
 * This function is responsible for setting or modifying environment variables.
 *
 * Return: Always 0 on success; otherwise, returns 1.
 */
int setenv_cmd(info_t *info)
{
	/* Check if the number of arguments is correct */
	if (info->argc != 3)
	{
		_errputs("Wrong number of arguments\n");
		return (1);
	}
	/* Call setenv_cmd function to set or modify the environment variable */
	if (setenv(info->argv[1], info->argv[2], 1) != 0)
	{
		perror("setenv");
		return (1);
	}
	return (0);
}

/**
 * unsetenv_cmd - Uninitialize an environment variable.
 *
 * @info: Pointer to a structure containing potential arguments,
 * used to maintain a consistent function prototype.
 *
 * This function is responsible for removing one or more environment variables
 * specified as command-line arguments in the 'info' structure.
 *
 * Return: Always 0.
 */
int unsetenv_cmd(info_t *info)
{
	int c;
	/* Check if too few arguments were provided */
	if (info->argc == 1)
	{
		_errputs("More arguments needed.\n");
		return (1);
	}
	/* Loop true d args n call unsetenv 2 remove each environment variable */
	for (c = 1; c < info->argc; c++)
	{
		if (unsetenv(info->argv[c]) != 0)
		{
			perror("unsetenv");
			return (1);
		}
	}
	return (0);
}

/**
 * populateEnv_list - Populate the environment linked list.
 *
 * @info: Pointer to a structure containing potential arguments,
 * used to maintain a consistent function prototype.
 *
 * Return: Always 0 to indicate successful completion.
 */
int populateEnv_list(info_t *info)
{
	size_t c;
	list_t *node = NULL; /* Initialize a linked list node */
	/* Loop true d 'environ' array to add envmt variables 2 d linked list */
	for (c = 0; environ[c]; c++)
		nodeadd_end(&node, environ[c], 0);
	info->env = node;
	return (0); /* Indicate successful completion */
}
