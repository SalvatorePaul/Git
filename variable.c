#include "myshell.h"

/**
 * lookFor_chain - Determines whether chaining should continue based on the last status.
 *
 * This function checks if chaining should continue based on the last command's status
 * and updates the buffer accordingly.
 *
 * @info: The parameter struct containing command information.
 * @buff: The character buffer to update.
 * @c: Address of the current position in the buffer.
 * @d: Starting position in the buffer.
 * @leng: Length of the buffer.
 *
 * Return: Void.
 */
void lookFor_chain(info_t *info, char *buff, size_t *c, size_t d, size_t leng)
{
    size_t a = *c;

   switch (info->cmd_buffer_type)
    {
        case CMND_AND:
            /* If the last command had a successful status, terminate the buffer. */
            if (info->status)
            {
                buff[d] = 0;
                a = leng;
            }
            break;

        case CMND_OR:
            /* If the last command had an unsuccessful status, terminate the buffer. */
            if (!info->status)
            {
                buff[d] = 0;
                a = leng;
            }
            break;

        default:
            /* Handle any other cases as needed. */
            break;
    }

    *c = a;
}

/**
 * isChainDelimiter - Checks if the current character in the buffer is a chain delimiter.
 *
 * This function examines the character in the buffer at the current position
 * to determine if it is a chain delimiter ('||', '&&', or ';').
 *
 * @info: The parameter struct containing command information.
 * @buff: The character buffer to analyze.
 * @currentPos: Address of the current position in the buffer.
 *
 * Return: 1 if the current character is a chain delimiter, 0 otherwise.
 */
int isChainDelimiter(info_t *info, char *buff, size_t *currentPos)
{
	size_t currentChar= *currentPos;

    if (buff[currentChar] == '|' && buff[currentChar + 1] == '|')
{
    buff[currentChar] = 0; /* Replace the first '|' with null terminator to split the command. */
    currentChar++; /* Increment currentChar to skip the second '|'. */
    info->cmd_buffer_type = CMND_OR; /* Set the command buffer type to CMD_OR for command chaining. */
}
else if (buff[currentChar] == '&' && buff[currentChar + 1] == '&')
{
    buff[currentChar] = 0; /* Replace the first '&' with null terminator to split the command. */
    currentChar++; /* Increment currentChar to skip the second '&'. */
    info->cmd_buffer_type = CMND_AND; /* Set the command buffer type to CMD_AND for command chaining. */
}
else if (buff[currentChar] == ';') /* Found the end of this command */
{
    buff[currentChar] = 0; /* Replace the semicolon with a null terminator to split the command. */
    info->cmd_buffer_type = CMND_CHAIN; /* Set the command buffer type to CMD_CHAIN for command chaining. */
}
else
{
    return (0); /* No chain delimiter found. */
}

*currentPos = currentChar; /* Update the current position pointer to currentChar. */
return (1); /* Chain delimiter found and processed. */

}

/**
 * sub_strn - Replaces a string with a new one.
 *
 * This function replaces the content of the old string with a new string.
 *
 * @former: A pointer to the address of the old string.
 * @new: The new string to replace the old one.
 *
 * Return: 1 if the replacement is successful, 0 otherwise.
 */
int sub_strn(char **former, char *new)
{
    /* Free the memory occupied by the old string. */
    free(*former);

    /* Update the pointer to the old string with the new string. */
    *former = new;

    /* Return 1 to indicate successful replacement. */
    return (1);
}

/**
 * replaceAlias - Replaces an alias in the tokenized string.
 *
 * This function searches for and replaces an alias in the command's arguments
 * based on the parameter struct information.
 *
 * @info: The parameter struct containing command information.
 *
 * Return: 1 if an alias is replaced, 0 otherwise.
 */
int replaceAlias(info_t *info)
{
    int aliasSearchLimit = 0; /* Limit the number of alias replacement attempts. */
    list_t *aliasNode;
    char *aliasValue;

    while (aliasSearchLimit < 10) /* Loop up to 10 times */
{
    /* Find any alias stored */
    aliasNode = find_node(info->alias, info->argv[0], '=');
    if (aliasNode == NULL) /* If no alias is found */
    {
        /* Return 0 to indicate failure */
        return (0);
    }
    else
    {
        free(info->argv[0]); /* Free the memory of the old alias */
        /* Find the character '=' in the alias */
        aliasValue = str_chr(aliasNode->str, '=');
        if (!aliasValue) /* If '=' is not found */
        {
            return (0);
        }
        aliasValue = dupStrn(aliasValue + 1); /* Duplicate the content after '=' */
        if (!aliasValue) /* If duplication fails */
        {
            return (0);
        }
        /* Update argv[0] with the new alias content */
        info->argv[0] = aliasValue;
        aliasSearchLimit++; /* Increment the counter */
    }
}

return (1);
}


/**
 * replaceVariables - Replaces variables in the tokenized string.
 *
 * This function searches for and replaces variables in the command's arguments
 * based on the parameter struct information.
 *
 * @info: The parameter struct containing command information.
 *
 * Return: 1 if variables are replaced, 0 otherwise.
 */
int replaceVariables(info_t *info)
{
	list_t *node;          /* Pointer to a linked list node containing environment variables. */
    int argumentIndex = 0;  /* Index for iterating through command arguments. */

    for (argumentIndex = 0; info->argv[argumentIndex]; argumentIndex++)
{
    if (info->argv[argumentIndex][0] != '$' || !info->argv[argumentIndex][1])
    {
        continue; /* Skip non-variable arguments */
    }

    if (!strcmp(info->argv[argumentIndex], "$?")) /* If the argument is "$?" */
    {
        sub_strn(&(info->argv[argumentIndex]), strdup(number_to_strn(info->status, 10, 0)));
        continue;
    }

    if (!strcmp(info->argv[argumentIndex], "$$")) /* If the argument is "$$" */
    {
        sub_strn(&(info->argv[argumentIndex]), strdup(number_to_strn(getpid(), 10, 0)));
        continue;
    }

    node = find_node(info->env, &info->argv[argumentIndex][1], '=');
    if (node) /* If the environment variable is found */
    {
        sub_strn(&(info->argv[argumentIndex]), strdup(str_chr(node->str, '=') + 1));
        continue;
    }

    sub_strn(&info->argv[argumentIndex], strdup("")); /* Replace with an empty string if not found */
}

return (0);

}
