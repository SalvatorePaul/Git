#include "myshell.h"

/**
 * handleInteract - Checks if the shell is in interactive mode.
 * @info: A pointer to the `info_t` structure that holds shell information.
 *
 * Return: 1 if the shell is in interactive mode, 0 otherwise.
 */
int handleInteract(info_t *info)
{
	/* Find out if the shell is active*/
	return (isatty(STDIN_FILENO) && info->readfd <= 2);
}

/**
 * check_delim - Check if a character is a delimiter
 * @c: The character to check for being a delimiter.
 * @delim: The delimiter string to compare 'c' against.
 * This function checks if a given character 'c' is a delimiter by comparing it
 * against the characters in the 'delim' string.
 * Return: 1 if 'c' is a delimiter according to 'delim', 0 otherwise.
 */
int check_delim(char c, char *delim)
{
	while (*delim)
		if (*delim++ == c)
			return (1); /* 'c' is a delimiter */
	return (0); /* 'c' is not a delimiter */
}

/**
 *check_alphabet- check if it is between 'a-z'
 *@c: The character to inputed and checked
 *Return: 1 if c is alphabetic, 0 otherwise
 */

int check_alphabet(int c)
{
	/* To check if it's an alphabetic character. */
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
		/* To check if it's not an alphabetic character. */
	else
		return (0);
}

/**
 *strn_to_integer - converts atoi
 *@s: string to be converted
 *Return: The integer value represented by the input string.
 */

int strn_to_integer(char *s)
{
	int output;
	int i;
	int sign = 1;
	unsigned int result = 0;
	int flag = 0;

	for (i = 0; s[i] != '\0' && flag != 2; i++)
	{
		switch (s[i])
		{
			case '-':
				sign *= -1;
				break;
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				flag = 1;
				result *= 10;
				result += (s[i] - '0');
				break;
			default:
				if (flag == 1)
					flag = 2;
				break;
		}
	}
	if (sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}
