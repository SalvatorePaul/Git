#include "myshell.h"

/**
 * dupstrn - Duplicate a string.
 *
 * This function duplicates the input string 'strn' by allocating memory for
 * a new string and copying the characters from 'strn' to the new string.
 *
 * @str: The source string to duplicate.
 *
 * Return: A pointer to the duplicated string, or NULL if 'strn' is NULL or
 *         if memory allocation fails.
 */
char *dupstrn(const char *strn)
{
	char *duplicate;    /* Declare a pointer for the duplicated string */
    int length = 0;     /* Initialize a variable to store the length of the input string */

    /* Check if the input string is NULL */
    if (strn == NULL)
        return (NULL);

    /* Calculate the length of the input string */
    while (*strn++)
        length++;

    /* Allocate memory for the duplicated string, including space for the null terminator */
    duplicate = malloc(sizeof(char) * (length + 1));

    /* Check if memory allocation was successful */
    if (!duplicate)
        return (NULL);

    /* Copy characters from the input string to the duplicated string in reverse order */
    for (length++; length--;)
        duplicate[length] = *--strn;

    return (duplicate);  /* Return a pointer to the duplicated string */
}
/**
 * wrt_chr - Write a character to stdout.
 *
 * @a: The character to print.
 *
 * Return:
 *   On success, returns 1.
 *   On error, returns -1, and errno is set.
 */
int wrt_chr(char a)
{
	static char outputBuffer[WRITE_BUFFER_SIZE];
    static int bufferIndex;

    /* Check if it's time to flush the buffer or if 'a' is a flush indicator. */
    if (a == BUFFER_FLUSH || bufferIndex >= WRITE_BUFFER_SIZE)
    {
        write(1, outputBuffer, bufferIndex);
        bufferIndex = 0;
    }

    /* Store the character in the output buffer if it's not a flush indicator. */
    if (a != BUFFER_FLUSH)
        outputBuffer[bufferIndex++] = a;

    return (1); /* Successful write. */
}
/**
 * cpyStrn - Copy a string from source to destination.
 *
 * @jack: Pointer to the destination where the string will be copied.
 * @jill: Pointer to the source string to be copied.
 *
 * Return: Pointer to the destination string 'jack'.
 *         If 'jack' is the same as 'jill' or 'jill' is NULL, returns 'jack' as is.
 */
char *cpyStrn(char *jack, char *jill)
{
    int a;

    /* Check for cases where copying is not necessary. */
    if (jack == jill || jill == NULL)
        return (jack);

    /* Use a loop to copy characters from 'jill' to 'jack'. */
    for (a = 0; jill[a]; a++)
    {
        jack[a] = jill[a];
    }

    /* Add a null terminator to the destination string. */
    jack[a] = '\0';

    return (jack);
}
/**
 * prnt_strn - Print an input string.
 *
 * This function prints the characters of the input string 'strn' to the standard
 * output using a for loop.
 *
 * @strn: The string to be printed.
 *
 * Return: Nothing.
 */
void prnt_strn(char *strn)
{
	int a = 0;

    if (!strn)
        return;  /* If the input string is NULL, do nothing */

    for (a = 0; strn[a] != '\0'; a++)  /* Iterate through the string */
    {
        wrt_chr(strn[a]);  /* Print each character of the string */
    }
}
