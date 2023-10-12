#include "myshell.h"

/**
 * infoSet - Initialize an info_t struct with relevant information.
 * This function initializes the info_t struct with data extracted from the
 * provided argument vector (av). It sets the program name, parses the argument
 * string (if available), and calculates the number of arguments (argc).
 * Additionally, it performs operations to replace aliases and variables.
 * @info: Pointer to the info_t struct to initialize
 * @av: Argument vector (command-line arguments).
 */
void infoSet(info_t *info, char **av)
{
	int a = 0; /* Initialize 'a' outside of the loop */

	info->fname = av[0]; /* Set the program name from the argument vector */
	if (info->arg)
	{
		/* Parse the argument string into an argument vector */
		info->argv = splitString(info->arg, " \t"); 
		if (!info->argv)
		{
			/* Handle memory allocation failure by creating a minimal argv */
			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = dupStrn(info->arg);
				info->argv[1] = NULL;
			}
		}
		/* Calculate the number of arguments (argc) */
		for (a = 0; info->argv && info->argv[a]; a++)
			;
		info->argc = a;

		/* Replace aliases and variables in the argument vector */
		replaceAlias(info);
		replaceVariables(info);
	}
}
/**
 * clear_info -  Reset fields within an info_t struct.
 * @info: Pointer to the info_t struct to reset.
 * This function resets specific fields within the info_t struct to their
 * initial states, effectively clearing them. It is typically used to prepare
 * the info_t struct for reuse or to initialize it to a clean state.
 */
void infoClear(info_t *info)
{
	/* Reset argument-related fields */
	info->arg = NULL;
	info->argv = NULL;
	/* Reset the path field */
	info->path = NULL;
	/* Reset the argument count (argc) to 0 */
	info->argc = 0;
}
/**
 * infoFree - Free fields within an info_t struct.
 * @info: Pointer to the info_t struct whose fields need to be freed.
 * @all:  A flag indicating whether to free all fields (1) or not (0).
 * This function deallocates memory associated with various fields within
 * the info_t struct. It can be used to selectively free specific fields or
 * free all fields by setting the 'all' flag to 1.
 */
 void infoFree(info_t *info, int all)
{/* Free memory allocated for 'argv' */
	free_strn(info->argv);
	info->argv = NULL;
	info->path = NULL;

	if (all)/* If 'all' flag is true */
	{/* If 'cmd_buffs' is not allocated */
	if (!info->cmd_buffer)
	/* Free memory allocated for 'arg' */
	free(info->arg);
	/* If 'env' is allocated */
	if (info->env)
	 /* Free memory for 'env' linked list */
	freeList(&(info->env));
	/* If 'history' is allocated */
	if (info->history)
	/* Free memory for 'history' linked list */
	freeList(&(info->history));
	 /* If 'alias' is allocated */
	if (info->alias)
	 /* Free memory for 'alias' linked list */
	freeList(&(info->alias));
	/* Free memory allocated for 'environ' */
	free_strn(info->environ);
	 /* Set 'environ' to NULL */
	info->environ = NULL;
	/* Free memory allocated for 'cmd_buffs' array */
	memfree((void **)info->cmd_buffer);
	/* If 'readfd' is greater than 2 */
	if (info->readfd > 2)
	/* Close the file descriptor 'readfd' */
	close(info->readfd);
	/* Write a character to flush the buffer */
	wrt_chr(BUFFER_FLUSH);
	}
}
