#include "myshell.h"

/**
 * free_strn - Frees memory allocated for an array of strings.
 * @p: A pointer to an array of strings to be freed.
 *
 * Return: No return value.
 */
void free_strn(char **p)
{
    char **a = p; /* Store the original pointer for later use */

    if (!p)
    {
        /* If 'p' is NULL, there's nothing to free, so return early */
        return;
    }

    for (; *p != NULL; p++)
    {
        /* Free each individual string in the array */
        free(*p);
    }

    free(a); /* Free the array itself */
}
/**
 * realloc_mem - Reallocates a block of memory.
 *
 * This function reallocates a block of memory pointed to by 'pt' from the
 * previous size 'pre_size' to the new size 'new_size'.
 *
 * @param pt: Pointer to the previous malloc'ated block.
 * @param pre_size: Byte size of the previous block.
 * @param new_size: Byte size of the new block.
 *
 * Return: Pointer to the reallocated block of memory.
 *         If 'new_size' is 0 and 'ptr' is not NULL, the memory is freed, and NULL is returned.
 *         If memory allocation fails, NULL is returned.
 */
void *realloc_mem(void *pt, unsigned int pre_size, unsigned int new_size)
{
	unsigned int i;
	char *p;

    if (!pt)
    {
        /* If 'pt' is NULL, allocate a new block of size 'new_size' */
        return malloc(new_size);
    }
    else if (new_size == 0)
    {
        /* If 'new_size' is 0, free the previous block and return NULL */
        free(pt);
        return NULL;
    }
    else if (new_size == pre_size)
    {
        /* If 'new_size' is equal to 'pre_size', return the original 'ptr' */
        return pt;
    }
    
    /* Allocate a new block of size 'new_size' */
    p = malloc(new_size);
    if (!p)
    {
        /* If memory allocation fails, return NULL */
        return NULL;
    }
    
    /* Copy data from the old block to the new block */
    pre_size = pre_size < new_size ? pre_size : new_size;
    for (i = 0; i < pre_size; i++)
    {
        p[i] = ((char *)pt)[i];
    }
    
    /* Free the old block */
    free(pt);
    
    return p;
}
/**
 * mem_set - Fills a memory area with a constant byte.
 *
 * This function fills the first 'n' bytes of the memory area pointed to by 'm'
 * with the constant byte 'b'.
 *
 * @param m: Pointer to the memory area to be filled.
 * @param b: The byte value to fill 'm' with.
 * @param n: The number of bytes to be filled.
 *
 * Return: A pointer to the memory area 'm'.
 */
char *mem_set(char *m, char b, unsigned int n)
{
    unsigned int c;

    for (c = 0; c < n; c++)
    {
        m[c] = b; /* Fill each byte of the memory area with 'b' */
    }

    return (m);
}
