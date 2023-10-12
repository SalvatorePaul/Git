#include "myshell.h"

/**
 * cmpstrn - Compare two strings lexicographically up to a specified length.
 *
 * @s1: Pointer to the first string.
 * @s2: Pointer to the second string.
 *
 * Return: 
 *   - Negative value if s1 is lexicographically less than s2.
 *   - Positive value if s1 is lexicographically greater than s2.
 *   - Zero if s1 and s2 are equal up to the specified length.
 */
int cmpstrn(const char *s1, const char *s2, size_t n)
{
	(void)n;
    /* Iterate through both strings as long as both are not null terminators. */
    for (; *s1 && *s2; s1++, s2++)
    {
        /* If characters at the current positions are different, return the
           difference (positive or negative). */
        if (*s1 != *s2)
            return (*s1 - *s2);
    }
    
    /* If we reached the end of both strings and they are equal up to this point,
       return 0. */
    if (*s1 == *s2)
        return (0);
    /* If one string is shorter than the other, consider the shorter one as
       lexicographically less. */
    else
        return (*s1 < *s2 ? -1 : 1);
}
/**
 * lenstrn - Calculate the length of a string.
 *
 * This function calculates the length of a given string by iterating through
 * each character until it reaches the null terminator ('\0').
 *
 * @s: The input string.
 *
 * Return: The integer length of the string, excluding the null terminator.
 */
int lenstrn(char *s)
{
    int a;  /* Declare the counter variable */

    if (!s)
        return (0);

    for (a = 0; s[a] != '\0'; a++)  /* Use a for loop to iterate until the null terminator */
    {
        /* No need to increment 'a' explicitly; it's done in the loop header */
    }

    return (a);  /* Return the calculated length of the string */
}
/**
 * catstrn - Concatenate two strings.
 *
 * @dest: Pointer to the destination string where the concatenation will occur.
 * @src: Pointer to the source string that will be appended to 'dest'.
 *
 * Return: Pointer to the resulting destination string after concatenation.
 */
char *catstrn(char *dest, char *src)
{
    char *result = dest;

    /* Move the 'dest' pointer to the end of the destination string. */
    for (; *dest; dest++)
        ;

    /* Copy characters from 'src' to 'dest' until the null terminator of 'src' is reached. */
    for (; *src; src++, dest++)
        *dest = *src;

    /* Append a null terminator to the concatenated string. */
    *dest = '\0';

    return (result);
}
/**
 * starts_with_strn - Check if 'jill' starts with 'jack'.
 *
 * This function checks if the 'jack' string begins with the 'jill' string.
 *
 * @jack: The string to search.
 * @jill: The substring to find.
 *
 * Return: If 'jill' is found at the beginning of 'jack', return the
 *         address of the next character in 'jack'. Otherwise, return NULL.
 */
char *starts_with_strn(const char *jack, const char *jill)
{
    for (; *jill; jill++, jack++)
    {
        if (*jill != *jack)
        {
            return (NULL);  /* Return NULL if the characters do not match */
        }
    }

    return (char *)jack;  /* Return the address of the next character in 'jack' */
}
