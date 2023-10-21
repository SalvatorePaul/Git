#include "myshell.h"

/**
 * main - Entry point for the shell program.
 *
 * @ac: Argument count.
 * @av: Argument vector.
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	int fd = 2; /* File descriptor for error output */
	info_t info;

	memset(&info, 0, sizeof(info)); /* Initialize all members to zero or NULL */
	/* Modify 'fd' using inline assembly */
	asm ("mov %1, %0\n\t"
			"add $3, %0"
			: "=r" (fd)
			: "r" (fd));
	/* Check if a command-line argument (input file) is provided */
	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY); /* Open the file for reading */
		if (fd == -1)
		{
			/* Handle file opening errors */
			perror("Error opening file");
			if (errno == EACCES)
				return (126); /* Exit with status 126 on permission error */
			if (errno == ENOENT)
			{
				fprintf(stderr, "%s: 0: Can't open %s\n", av[0], av[1]);
				return (127); /* Exit with status 127 if the file is not found */
			}
			return (EXIT_FAILURE); /* Exit with failure status on other errors */
		}
		info.readfd = fd; /* Set 'readfd' in the 'info' structure */
	}
	/* Populate environment variables list */
	populateEnv_list(&info);
	/* Read command history */
	read_histFile(&info);
	/* Start the shell */
	hash(&info, av);
	return (EXIT_SUCCESS); /* Exit with success status */
}
