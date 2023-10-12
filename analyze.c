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
 * cmpstrn - Compare two strings up to a specified length.
 * @s1: The first string to compare.
 * @s2: The second string to compare.
 * @n: The maximum number of characters to compare.
 *
 * Return: 0 if the strings are equal up to the specified length, a positive
 *         value if s1 is greater, or a negative value if s2 is greater.
 */
int cmpstrn(const char *s1, const char *s2, size_t n)
{
    while (n > 0) {
        if (*s1 != *s2)
            return (*s1 - *s2);
        if (*s1 == '\0')
            return 0;
        s1++;
        s2++;
        n--;
    }
    return 0;
}

/**
 * lookFor_path - Find the full path of a command in the PATH string.
 * @info: Pointer to the info struct.
 * @pathstrn: The PATH string to search for the command.
 * @command: The command to find.
 *
 * Return: A dynamically allocated string containing the full path of the command
 * if found, or NULL if the command is not in the PATH.
 */
char *lookFor_path(info_t *info, char *pathstrn, char *command)
{
	int a;
	char *path;
	int curr_pos = 0;

    /* Check for a NULL PATH string. */
    if (!pathstrn)
        return NULL;

    /* Check if the command starts with "./" and is a valid command. */
    if ((lenstrn(command) > 2) && cmpstrn(command, "./", 2) == 0)
    {
        if (cmd_exists(info, command))
            return dupStrn(command);  /* Duplicate and return the command. */
        else
            return NULL;  /* Command is not valid. */
    }

    for (a = 0; pathstrn[a] != '\0'; a++)
    {
        if (pathstrn[a] == ':')
        {
            /* Extract a path segment from pathstrn. */
            path = dup_chars(pathstrn + curr_pos, 0, a - curr_pos);  /* Calculate the length correctly. */

            /* If the extracted path is empty, append the command directly. */
            if (!*path)
                path = dupStrn(command);
            else
            {
                /* Otherwise, concatenate the command to the path with a '/' separator. */
                path = realloc(path, lenstrn(path) + lenstrn(command) + 2);
                strcat(path, "/");
                strcat(path, command);
            }

            /* Check if the constructed path is a valid command. */
            if (cmd_exists(info, path))
                return path;

            free(path);  /* Free allocated memory for path. */
            curr_pos = a + 1;  /* Update the current position. */
        }
    }

    /* Handle the last path segment after the loop. */
    path = dup_chars(pathstrn + curr_pos, 0, strlen(pathstrn) - curr_pos);

    /* If the extracted path is empty, append the command directly. */
    if (!*path)
        path = strdup(command);
    else
    {
        /* Otherwise, concatenate the command to the path with a '/' separator. */
        path = realloc(path, lenstrn(path) + lenstrn(command) + 2);
        strcat(path, "/");
        strcat(path, command);
    }

    /* Check if the constructed path is a valid command. */
    if (cmd_exists(info, path))
        return path;

    free(path);  /* Free allocated memory for path. */

    /* Command not found in the PATH. */
    return NULL;
}
