#include "myshell.h"

/**
 * errputchar - Write a character to the standard error (stderr).
 * @c: The character to be written to stderr.
 * This function writes a single character 'c' to the standard error (stderr)
 * stream. It uses a static buffer to efficiently collect characters before
 * writing them to stderr, flushing the buffer when necessary.
 *
 * Return: On success, it returns 1. On error, -1 is returned, and errno is
 *         set appropriately.
 */
int errputchar(char c)
{
	static char buff[WRITE_BUFFER_SIZE];
	static int a;

	if (c == BUFFER_FLUSH || a >= WRITE_BUFFER_SIZE)
	{
		write(2, buff, a);
		a = 0;
	}
	if (c != BUFFER_FLUSH)
		buff[a++] = c;
	return (1);
}
/**
 *putsfd - Print a string to a specified file descriptor.
 * @str: The string to be printed.
 * @fd: The file descriptor to write to.
 * This function prints the characters from the 'str' string to the file
 * descriptor 'fd' and returns the number of characters written.
 *
 * Return: The number of characters successfully written.
 */
int putsfd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	for (; *str; str++)
	{
		 /* Write the character to the file descriptor using _putfnd() */
		i += _putfnd(*str, fd);
	}
	return (i);
}
/**
 * _putfd - Write a character to the specified file descriptor.
 * @c: The character to write.
 * @fd: The file descriptor to write to.
 * This function writes the character 'c' to the file descriptor 'fd'. If the
 * internal buffer is full or the character is a flush signal (BUF_FLUSH), the
 * buffer is written to 'fd', and the buffer is reset. Returns 1 on success,
 * or -1 on error with appropriate errno setting.
 *
 * Return: 1 on success, -1 on error.
 */
int _putfd(char c, int fd)
{
	static char buff[WRITE_BUFFER_SIZE];
	static int a;

	if (c == BUFFER_FLUSH || a >= WRITE_BUFFER_SIZE)
	{
		write(fd, buff, a);
		a = 0;
	}
	if (c != BUFFER_FLUSH)
		buff[a++] = c;
	return (1);
}
/**
 *errputs - Print an input string to the standard error stream.
 * @str: The string to be printed.
 * This function prints the characters from the 'str' string to the standard
 * error stream (stderr).
 *
 * Return: It does not return a value (void).
 */
void errputs(char *str)
{
	int a = 0; 
	
	/* Check if the input string is NULL, in which case, do nothing. */
	if (!str)
		return;
		/* Iterate through the characters in 'str' until the null-terminator is encountered. */
	while (str[a] != '\0')
	{
		/* Call _eputchar to write the current character to the standard error stream (stderr). */
		errputchar(str[a]);
		a++;
	}
}
