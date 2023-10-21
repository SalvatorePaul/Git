#include "myshell.h"

/**
 * strn_cat - Concatenate two strings up to 'n' characters.
 * @dest: Pointer to the destination string.
 * @src: Pointer to the source string to be appended.
 * @n: The maximum number of bytes to be appended from 'src'.
 *
 * Description: This function adds characters from d 'src' string to the 'dest'
 * string up to a maximum of 'n' bytes, including the null terminator. It first
 * calculates the length of the 'dest' string and then appends characters from
 * 'src' until 'n' characters have been appended or until the end of 'src' is
 * reached (null terminator). If 'n' is greater than the remaining space in
 * 'dest' after the current string length, it will only append up to the
 * remaining space.
 *
 * Return: A pointer to the destination string 'dest' after appending 'src'.
 */
char *strn_cat(char *dest, char *src, int n)
{
	int a;
	int b;
	char *s = dest;

	for (a = 0; dest[a] != '\0'; a++) /* Find the end of the destination string */
		;

	for (b = 0; src[b] != '\0' && b < n; b++)
	{
		dest[a] = src[b]; /* Copy a character from src to dest */
		a++; /* Increment the destination index */
	}
	/* Ensure that dest is null-terminated if fewer than n chars are copied */
	if (b < n)
		dest[a] = '\0';

	return (s);
}

/**
 * strn_cpy - Copy characters from source to destination string.
 * @dest: Pointer to the destination string where characters will be copied.
 * @src: Pointer to the source string from which characters will be copied.
 * @n: The maximum number of characters 2 b copied, includin d null terminator.
 *
 * Description: This function copies 'n' characters from the 'src' 2 d 'dest'
 * string. It stops copying when 'n' characters have been copied, or wen d end
 * of d 'src' string is reached, whichever comes first. If 'n' is greater than
 * the length of 'src', the remaining characters in 'dest' will be filled with
 * null ('\0') characters to reach a total of 'n' characters.
 *
 * Return: A pointer to the destination string 'dest'.
 */
char *strn_cpy(char *dest, char *src, int n)
{
	int a;
	int j;
	char *s = dest;

	a = 0;
	while (src[a] != '\0' && a < n - 1)
	{
		/* Copy characters from src to dest */
		dest[a] = src[a];
		a++;
	}
	/* Check if less than n characters were copied */
	if (a < n)
	{
		/* Copy the value of a into j */
		j = a;
		while (j < n)
		{
			dest[j] = '\0';
			j++;
		}
	}
	return (s);
}

/**
 * str_chr - Locate a character in a string.
 * @s: Pointer to the string to be searched.
 * @c: The character to search for within the string.
 *
 * Description: This function looks 4 d 1st occurrence of d character 'c' in
 * the string 's'. It iterates through the string until 'c' is found or
 * until the end of the string is reached (the null terminator '\0' is
 * encountered). If 'c' is found, it returns a pointer to the memory location
 * where 'c' is located within 's'. If 'c' is not found, it returns NULL.
 *
 * Return: A pointer to the first occurrence of 'c' within 's' or NULL if
 *         'c' is not found in 's'.
 */
char *str_chr(char *s, char c)
{
	while (*s != '\0')
	{
		if (*s == c)
			return (s);
		s++;
	}
	return (NULL);
}
