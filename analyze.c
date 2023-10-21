#include "myshell.h"

/**
 * cmd_exists - Checks if a file exists and is an executable command.
 * @info: The info struct (unused in this function).
 * @path: The path to the file to be checked.
 *
 * Return: 1 if the file exists and is a regular file (executable command),
 *         otherwise returns 0.
 */
int cmd_exists(info_t *info, char *path)
{
	struct stat st;
	/* Suppress unused parameter warning */
	(void)info;
	/* Check if the path is NULL or if calling `stat` fails */
	if (!path || stat(path, &st))
		return (0);
	/* Check if the file is a regular file */
	if (S_ISREG(st.st_mode))
	{
		/* File is considered an executable command */
		return (1);
	}
	/* File is not an executable command */
	return (0);
}

/**
 * dup_chars - Duplicates characters from a string
 * @pathstrn: The PATH string
 * @begin: Starting index
 * @end: Stopping index
 *
 * Return: A pointer to a new buffer containing duplicated characters
 */
char *dup_chars(char *pathstrn, int begin, int end)
{
	int a = 0;
	int b = 0;      /* Loop counters for input and output buffers */
	static char buffer[1024]; /* Static buffer to store duplicated characters */

	for (b = 0, a = begin; a < end; a++)
	{
		if (pathstrn[a] != ':')
		{
			buffer[b++] = pathstrn[a]; /* Copy non-colon characters to the buffer */
		}
	}

	buffer[b] = '\0'; /* Null-terminate the output buffer */

	return (buffer);
}
/**
 * lookFor_path - Find the full path of a command in the PATH string.
 * @info: Pointer to the info struct.
 * @pathstrn: The PATH string to search for the command.
 * @command: The command to find.
 *
 * Return: string containing the full path of the command
 */
char *lookFor_path(info_t *info, char *pathstrn, char *command)
{
	int a;
	char *path;
	int curr_pos = 0;

	if (!pathstrn)
	return (NULL);
	if ((lenstrn(command) > 2) && cmpstrn(command, "./", 2) == 0)
	{
	if (cmd_exists(info, command))
	return (command);/* Duplicate and return the command. */
	}
	for (a = 0; pathstrn[a] != '\0'; a++)
	{
	if (pathstrn[a] == ':')
		{
	path = dup_chars(pathstrn + curr_pos, 0, a - curr_pos);
	if (!*path)
	strcat(path, command);
	else
	path = realloc(path, lenstrn(path) + lenstrn(command) + 2);
	strcat(path, "/");
	strcat(path, command);
	if (cmd_exists(info, path))
	return (path);
	free(path);/* Free allocated memory for path. */
	curr_pos = a + 1;/* Update the current position. */
		}
	}
	path = dup_chars(pathstrn + curr_pos, 0, strlen(pathstrn) - curr_pos);
	if (!*path)
	path = strdup(command);
	else
	path = realloc(path, lenstrn(path) + lenstrn(command) + 2);
	strcat(path, "/");
	strcat(path, command);
	if (cmd_exists(info, path))
	return (path);
	free(path);
	return (NULL);
}
