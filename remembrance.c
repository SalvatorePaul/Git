#include "myshell.h"

/**
 * memfree - Frees a pointer and sets it to NULL.
 *
 * @ptr: Address of the pointer to free.
 *
 * Return: 1 if memory was freed and pointer set to NULL, 0 otherwise.
 */
int memfree(void **ptr)
{
	if (ptr != NULL && *ptr != NULL)
	{
		free(*ptr);
		*ptr = NULL;
		return (1); /* Memory freed, and pointer set to NULL */
	}
	return (0); /* No memory to free or pointer is already NULL */
}
