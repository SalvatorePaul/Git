#include "myshell.h"

/**
 * search_builtin - Finds and executes a builtin command.
 * @info: Pointer to the parameter and return info struct.
 *
 * Return:
 * -1 if builtin not found,
 *  0 if builtin executed successfully,
 *  1 if builtin found but not successful,
 * -2 if builtin signals exit()
 */
int search_builtin(info_t *info)
{
	int index, builtin_return = -1;

	/* Define a table of built-in commands and their corresponding functions */
	builtin_table builtintbl[] =
	{
		{"cd", cd},
    {"exit", exit_cmd},
    {"help", chnge_dir},
    {"history", hist_cmd},
    {"alias", alias_cmd},
    {"env", env_cmd},
    {"setenv", setenv_cmd},
    {"unsetenv", unsetenv_cmd},
    {NULL, NULL}
	};

	/* Iterate through the table to find a matching built-in command */
	for (index = 0; builtintbl[index].type; index++)
	{
		size_t n = 8;
		if (cmpstrn(info->argv[0], builtintbl[index].type, n) == 0)
		{
			info->line_count++;
			builtin_return = builtintbl[index].func(info);
			break;
		}
	}

	/* Return the result of the built-in command execution */
	return builtin_return;
}
/**
 * hash - Main shell loop that handles command execution.
 * @info: Pointer to the parameter and return info struct.
 * @av: Argument vector from main().
 *
 * Return:
 * 0 on success, 1 on error, or an error code.
 */
int hash(info_t *info, char **av)
{
    int builtin_ret = 0;
    ssize_t read_status = 0;

    /* Main shell loop */
    for (; read_status != -1 && builtin_ret != -2; infoClear(info))
    {
        /* Print shell prompt if in interactive mode */
        if (handleInteract(info))
            prnt_strn("$ ");

        /* Flush the error buffer */
        _errputchar(BUFFER_FLUSH);

        /* Read user input */
        read_status = inputGet(info);

        if (read_status != -1)
        {
            /* Set command-related information */
            infoSet(info, av);

            /* Check if it's a built-in command */
            builtin_ret = search_builtin(info);
            if (builtin_ret == -1)
                /* Execute non-built-in commands */
                find_command(info);
        }
        else if (handleInteract(info))
            /* Print a newline for readability */
            wrt_chr('\n');

        /* Free allocated memory after processing */
        infoFree(info, 0);
    }

    /* Write command history */
    write_histFile(info);

    /* Free memory related to command history */
    infoFree(info, 1);

    /* Exit with the status code if not in interactive mode and there's a status */
    if (!handleInteract(info) && info->status)
        exit(info->status);

    /* Handle the case where builtin_ret is -2 */
    if (builtin_ret == -2)
    {
        /* Exit with the status code if no specific error number */
        if (info->err_number == -1)
            exit(info->status);
        /* Exit with the specified error number */
        exit(info->err_number);
    }

    /* Return the result of command execution */
    return (builtin_ret);
}
/**
 * forkExe_command - Forks a child process to execute a command using execve.
 *
 * @info: A pointer to the 'info' struct that stores all information.
 *
 * Return: Void (empty)
 */
void forkExe_command(info_t *info)
{
    pid_t child_pid;

    child_pid = fork();

    switch (child_pid)
    {
        case -1:
            /* Error handling - Print an error message */
            perror("Error:");
            return;
        case 0:
            /* Child process */
            if (execve(info->path, info->argv, environ) == -1)
            {
                infoFree(info, 1);
                switch (errno)
                {
                    case EACCES:
                        /* Permission denied */
                        exit(126);
                    default:
                        exit(1); /* Exec error */
                }
            }
            break;
        default:
            /* Parent process - Waits for the child to complete */
            wait(&(info->status));
            if (WIFEXITED(info->status))
            {
                info->status = WEXITSTATUS(info->status);
                switch (info->status)
                {
                    case 126:
                        /* Access denied */
                        printErro(info, "Prohibited\n");
                        break;
                    default:
                        break;
                }
            }
            break;
    }
}
/**
 * find_command - Search for a command in the PATH environment variable.
 *
 * @info: A pointer to the parameter and return information struct.
 *
 * This function searches for the specified command in the directories
 * listed in the PATH environment variable and executes it if found.
 * If the command is not found, it sets an error status and prints an error message.
 */
void find_command(info_t *info)
{
	char *path = NULL;
int i;
int k;

info->path = info->argv[0];

/* Increment line count if linecount_flag is set */
if (info->linecount_flag == 1)
{
    info->line_count++;
    info->linecount_flag = 0;
}

/* Count the number of non-delimiter arguments */
for (i = 0, k = 0; info->arg[i]; i++)
{
    if (!check_delim(info->arg[i], " \t\n"))
    {
        k++;
    }
}

/* If there are no arguments, return without further processing */
if (!k)
{
    return;
}

/* Find the full path to the command */
path = lookFor_path(info, get_env(info, "PATH="), info->argv[0]);

/* If the path is found, update the command path and fork a new process */
if (path)
{
    info->path = path;
    forkExe_command(info);
}
else
{
    /* Check if the command can be executed */
    if (handleInteract(info) || get_env(info, "PATH=") || info->argv[0][0] == '/')
    {
        if (cmd_exists(info, info->argv[0]))
        {
            forkExe_command(info);
        }
        else if (*(info->arg) != '\n')
        {
            info->status = 127;
            printErro(info, "nonexistent\n");
        }
    }
}
}
