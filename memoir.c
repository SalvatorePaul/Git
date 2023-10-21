#include "myshell.h"
#define HIST_FILE "memoir.c"

/**
 * retrieve_histFile - Retrieves the path to the history file.
 *
 * This function constructs and returns a dynamically allocated string that
 * contains the full path to the history file, based on the user's home
 * directory and a predefined file name (HIST_FILE).
 * @info: Pointer to the parameter struct.
 *
 * Return: A dynamically allocated string containing the history file path,
 * or NULL on failure.
 */
char *retrieve_histFile(info_t *info)
{
	char *buffer;
	char *dir;

	/* Retrieve the user's home directory */
	dir = get_env(info, "HOME=");
	if (!dir)
		return (NULL);

	/* Allocate memory for the full file path */
	buffer = malloc(sizeof(char) * (lenstrn(dir) + lenstrn(HIST_FILE) + 2));
	if (!buffer)
		return (NULL);

	/* Initialize the buffer */
	buffer[0] = 0;

	/* Copy the home directory path to the buffer */
	cpyStrn(buffer, dir);

	/* Concatenate the '/' separator */
	catstrn(buffer, "/");

	/* Concatenate the history file name (HIST_FILE) */
	catstrn(buffer, HIST_FILE);

	return (buffer); /* Return the dynamically allocated file path */
}

/**
 * write_histFile - Creates a new file or appends to an existing file
 *                  to store historical data.
 * @info: Pointer to the parameter struct containing historical data.
 *
 * Return: 1 on success, -1 on failure.
 */
int write_histFile(info_t *info)
{
	list_t *node = NULL;
	ssize_t fd;
	char *filename = retrieve_histFile(info);

	/* Check if the filename is valid */
	if (!filename)
		return (-1);

	/* Open or create the file with appropriate permissions */
	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);

	/* Check if the file opening was successful */
	if (fd == -1)
		return (-1);

	/* Write historical data to the file */
	for (node = info->history; node; node = node->next)
	{
		_putsfnd(node->str, fd); /* Write a string to the file */
		_putfnd('\n', fd);      /* Write a newline character to the file */
	}
	fsync(fd); /* Flush the file buffer */
	close(fd);  /* Close the file */

	return (1); /* Return success */
}

/**
 * renumberHist - Renumbers the history linked list after changes.
 * This function iterates through the history linked list and updates the 'num'
 * field of each node to assign new sequential numbers to the entries.
 * @info: Pointer to the structure containing historical data.
 *
 * Return: The new 'histcount' after renumbering.
 */
int renumberHist(info_t *info)
{
	int a = 0;
	list_t *node = info->history;

	/* Iterate through the history list and update 'num' field */
	while (node)
	{
		node->num = a++;
		node = node->next;
	}

	/* Update 'histcount' in the 'info' structure */
	info->historycount = a;

	return (info->historycount); /* Return the new 'histcount' */
}

/**
 * read_histFile - Reads history entries from a file.
 * This function reads historical data from a file, parses entries, populates
 * the history list in the 'info' structure. It  ensures that the history list
 * does not exceed a maximum size.
 * @info: Pointer to the structure containing historical data.
 *
 * Return: The updated 'histcount' on success, 0 otherwise.
 */
int read_histFile(info_t *info)
{
	int a, last = 0, linecount = 0;
	char *buffer = NULL, *filename = retrieve_histFile(info);
	struct stat st;
	ssize_t fd, rdlen, fsize = 0;

	if (!filename)
		return (0);
	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buffer = malloc(sizeof(char) * (fsize + 1));
	if (!buffer)
		return (0);
	rdlen = read(fd, buffer, fsize);
	buffer[fsize] = 0;
	if (rdlen <= 0)
		return (free(buffer), 0);
	close(fd);
	for (a = 0; a < fsize; a++)
		if (buffer[a] == '\n')
		{
			buffer[a] = 0;
			build_histList(info, buffer + last, linecount++);
			last = a + 1;
		}
	if (last != a)
		build_histList(info, buffer + last, linecount++);
	free(buffer);
	info->historycount = linecount;
	while (info->historycount-- >= INT_MAX)
		nodeDelete_at_index(&(info->history), 0);
	renumberHist(info);
	return (info->historycount);
}

/**
 * build_histList - Adds an entry to a history linked list.
 * This function appends a new node to a linked list.
 * @info: A pointer to the structure containing potential arguments. Used
 *       to maintain historical data.
 * @buff: A buffer containing the data to be added to the history.
 * @linecount: The current line count in the history list.
 *
 * Return: Always 0
 */
int build_histList(info_t *info, char *buff, int linecount)
{
	list_t *node = info->history;
	list_t *new_node;

	/* Traverse the history list to find the last node */
	while (node && node->next)
	{
		node = node->next;
	}
	/* Create a new node and add it to the end of the history list */
	new_node = nodeadd_end(&(info->history), buff, linecount);
	if (new_node == NULL)
		return (-1); /* Error handling: Unable to create a new node */

	if (node)
		node->next = new_node;
	else
		info->history = new_node;

	return (0);
}
