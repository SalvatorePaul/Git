#include "myshell.h"

/**
 * chnge_dir - Changes the current directory of the process.
 * @info: Pointer to a structure containing potential function arguments and context.
 * This function is intended to modify the current working directory of the process,
 * but it is currently not fully implemented and serves as a placeholder.
 *
 * Return: Always returns 0 to indicate success.
 */
int chnge_dir(info_t *info)
{
	char **arg_ary;

	arg_ary = info->argv;
	prnt_strn("help notification works. Function not yet implemented \n");
	if (0)
		prnt_strn(*arg_ary); /* temp att_unused workaround */
	return (0);
}

/**
 * cd - Change the current directory of the shell process.
 * @info: Pointer to the inform_t struct with command information.
 *
 * This function handles changing the working directory of the shell process.
 * It interprets different command arguments to navigate the directory
 * structure. It supports changing to the home directory, switching to the
 * previous directory ("-"), and providing a specific directory path. The
 * function also updates relevant environment variables to reflect the changes
 * in the current and previous directories.
 *
 * Return: Always returns 0 to indicate success.
 */
int cd(info_t *info)
{
	char buff[1024];
	char *dir;
	char *s;
	int cd_ret;
	/* Get the current working directory. */
	s = getcwd(buff, sizeof(buff));
	if (!s)
		prnt_strn("Implemented:>>'getcwd' error message<<\n");
	if (!info->argv[1])/* Check if no directory argument is provided. */
	{dir = get_env(info, "HOME=");/* The home directory from env vars.*/
		if (!dir)
			cd_ret = chdir((dir = get_env(info, "PWD=")) ? dir : "/");
		else
			cd_ret = chdir(dir);/* Change to the home directory. */
	}
	else if (strcmp(info->argv[1], "-") == 0) /* Find the argument "-" */
	{
		if (!get_env(info, "OLDPWD="))/* Check if "OLDPWD" is not set. */
		{prnt_strn(s);/* Print the current working directory. */
			wrt_chr('\n');
			return (1);/* Return with success. */
		}
		wrt_chr('\n');/* Print the "OLDPWD" environment variable. */
		prnt_strn(get_env(info,	"OLDPWD=")), wrt_chr('\n');
		/* Change to "OLDPWD" or root if not set. */
		cd_ret = chdir((dir = get_env(info, "OLDPWD")) ? dir : "/");
	}
	else/* Change to the specified directory argument. */
		cd_ret = chdir(info->argv[1]);
	if (cd_ret == -1)/* Check if 'chdir' returned an error. */
	{/* Print an error message. */
		printErro(info, "Error: can't cd");
		_errputs(info->argv[1]), _errputchar('\n');/*Print the dir*/
	}
	else
	{/* Set "OLDPWD" to the previous current directory. */
		setenv("OLDPWD", get_env(info, "PWD="), 1); /* Update "OLDPWD" */
		setenv("PWD", getcwd(buff, sizeof(buff)), 1); /* Set the new "PWD" */
	}
	return (0);
}
	
/**
 * exit_cmd - exits the shell program.
 * @info: A structure containing potential arguments and context.
 *  Return: Exits with specified exit status if valid,
 *         otherwise sets an error status and returns a non-zero value.
 */
int exit_cmd(info_t *info)
{
	int exit_check;

	if (info->argv[1])  /* check if there is an exit arguement */
	{
		exit_check = err_atoi(info->argv[1]); /* Convert exit status to an integer. */
		if (exit_check == -1)
		{
			info->status = 2;
			printErro(info, "Illegal number: ");
			_errputs(info->argv[1]);
			_errputchar('\n');
			return (1);
		}
		info->err_number = err_atoi(info->argv[1]);
		return (-2); /* Indicates exit with the specified status */
	}
	info->err_number = -1;
	return (-2); /* Indicates exit with a default status */
}
