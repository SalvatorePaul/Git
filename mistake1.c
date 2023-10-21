#include "myshell.h"

/**
 * number_to_strn - Convert a long integer to a string rep in a specified base.
 * @number: The number to be converted.
 * @base: The numeric base for the conversion (e.g. 16 for hexadecimal).
 * @flags: Flags controlling the conversion
 *
 * Return: A pointer to the resulting string.
 */
char *number_to_strn(long int number, int base, int flags)
{
	char signal = 0;
	char *ptr;
	static char *arr, buff[50];
	unsigned long n = number;

	if (!(flags & CONVERTING_UNSIGNED) && number < 0)
	{
		n = -number;
		signal = '-';

	}
	arr = flags & CONVERTING_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buff[49];
	*ptr = '\0';

	do	{
		*--ptr = arr[n % base];
		n /= base;
	} while (n != 0);

	if (signal)
		*--ptr = signal;
	return (ptr);
}
/**
 * err_atoi - Converts a string to an integer.
 * @s: the string to be converted
 * Return: The converted integer if successful, or -1 on error.
 *         Returns 0 if no numbers are found in the string.
 */
int err_atoi(char *s)
{
	int b = 0;
	unsigned long int product = 0;

	if (*s == '+')
		s++;  /* TODO: why does this make main return 255? */
	for (b = 0;  s[b] != '\0'; b++)
	{
		switch (s[b])
	{
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	/* Multiply the current answer by 10 */
	product *= 10;
	/* Add the digit to the answer */
	product += (s[b] - '0');
	if (product > INT_MAX)
	/* Non-digit character seen, return error */
	{
	return (-1);
	}
	goto end_switch; /* Use a label and goto to leave the switch */

	default:
	return (-1);
	goto end_switch; /* Use a label and goto to leave the switch */
	}
	}
end_switch:
	return (product);
}
/**
 * print_d - function prints a decimal (integer) number (base 10)
 * @input: the input
 * @fd: the filedescriptor to write to
 *
 * Return: number of characters printed
 */
int print_d(int input, int fd)
{
	unsigned int _abs_, current;
	int (*_putchar)(char) = _putchar;
	int b, count = 0;

	if (fd == STDERR_FILENO)
		_putchar = _errputchar;
	if (input < 0)
	{
		_abs_ = -input;
		_putchar('-');
		count++;
	}
	else
		_abs_ = input;
	current = _abs_;
	for (b = 1000000000; b > 1; b /= 10)
	{
		if (_abs_ / b)
		{
			_putchar('0' + current / b);
			count++;
		}
		current %= b;
	}
	_putchar('0' + current);
	count++;

	return (count);
}
/**
 * remv_comnt - Replace the first instance of '#' with '\0' in a string.
 * @buffer: Address of the string to modify.
 *
 * Return: Always 0.
 */
void remv_comnt(char *buffer)
{
	int b;

	for (b = 0; buffer[b] != '\0'; b++)
		if (buffer[b] == '#' && (!b || buffer[b - 1] == ' '))
		{
			buffer[b] = '\0';
			break;
		}
}

/**
 * printErro - Print an error message.
 * @info: Pointer to the parameter and return info struct.
 * @estr: String containing the specified error type.
 * This function prints an error message in the format:
 * "<filename>: <line number>: <program name>: <error message>"
 * Return: 0 if no numbers in string, converted number otherwise
 *        -1 on errorS
 */
void printErro(info_t *info, char *estr)
{
	fprintf(stderr, "%s: %d: %s: %s\n",
			info->fname, info->line_count, info->argv[0], estr);
}
