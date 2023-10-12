#ifndef MY_SHELL_H
#define MY_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/stat.h>
#include <string.h>


/* Macros for buffer management */
#define BUFFER_FLUSH -1  /* Use BUFFER_FLUSH to indicate a buffer flush operation */
#define READ_BUFFER_SIZE 1024  /* READ_BUFFER_SIZE defines the size of the read buffer */
#define WRITE_BUFFER_SIZE 1024  /* WRITE_BUFFER_SIZE defines the size of the write buffer */


/* command chaining modes*/
#define CMND_CHAIN	3 /* Represents command chaining mode */
#define CMND_AND		2 /* Represents command AND mode */
#define CMND_OR		1 /* Represents command OR mode */
#define CMND_NORM	0 /* Represents normal command mode */



/* converting number() */
#define CONVERTING_UNSIGNED	2 /* Converting to unsigned number */
#define CONVERTING_LOWERCASE 1 /* Coverting to lowercase */


/* Configuration Flags for Input Parsing */
#define USING_STRTOK 0 /* Use strtok() for input parsing */
#define USING_GETLINE 0 /* Use getline() for input parsing */

/* File Name for Command History */
#define FILE_HISTORY	".simple_shell_history" /* Name of the command history file */
#define MAX_HISTORY	4096 /* Maximum number of commands to store in history */

/* External Environment Variable Array */
extern char **environ; /* Pointer to an array of environment variables */


/**
 * struct liststr - singly linked list
 * @str: a string
 * @num: number
 * 
 * @next: pointer to the next node
 */
typedef struct liststr
{
	char *str;
	int num;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - Container for holding various parameters to be passed
 *                  into a function, enabling a consistent prototype for
 *                  a function pointer struct
 *@arg: A string obtained from getline, containing function arguments
 *@history: Linked list for command history.
 *@alias: Linked list for command aliases.
 *@path: a string path for the current command
 *@env: Linked list for environment variables.
 *@fname: The filename of the program
 *@argv: An array of strings created from the 'arg' field
 *@environ: A customized modified copy of the environment from 'env'
 *@cmd_buffer: Address of a pointer to 'cmd_buf'; set to true when chaining
 *@argc: The count of arguments
 *@linecount_flag: Indicates whether to count this input line
 *@line_count: The count of encountered errors
 *@err_number: The error code for exit() calls
 *@status: The return status of the most recently executed command
 *@cmd_buffer_type: Type of command buffer (e.g., ||, &&, ;)
 *@env_changed: Set to true if the environment was altered
 *@readfd: The file descriptor from which to read line input
 *@historycount: The count of history line numbers
 */
typedef struct passinfo
{
	char *arg;
	list_t *history;
	list_t *alias;
	char *path;
	list_t *env;
	char *fname;
	char **argv;
	char **environ;
	char **cmd_buffer;
	int argc;
	int linecount_flag;
	unsigned int line_count;
	int err_number;
	int status;
	int cmd_buffer_type;
	int env_changed;
	int readfd;
	int historycount;
} info_t;

/* Initialize an inform_t structure with default values */
#define INFO_INIT \
{NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, 0, 0, 0}

/**
 *Struct builtin - stores a built-in command string and its corresponding function pointer.
 *
 *@type: Pointer to the built-in command string.
 *@func: Pointer to the function handling the built-in command.
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

/*  Function Declaration: loophsh.c */
int loophsh(char **);

/* bug handler */
int _errputchar(char); /* Write a character to stderr */
int _putsfnd(char *str, int fd); /* Write a string to a file descriptor */
int _putfnd(char c, int fd); /* Write a character to a file descriptor */
void _errputs(char *); /* Print error message to stderr */

/* string1 declaration*/
char *dupStrn(const char *); /* Duplicate a string */
int wrt_chr(char); /* Write a character */
char *cpyStrn(char *, char *); /* Copy a string */
void prnt_strn(char *); /* Print a string */


/* Exits declaration*/
char *strn_cat(char *, char *, int); /* Concatenate part of a string */
char *strn_cpy(char *, char *, int); /* Copy part of a string */
char *str_chr(char *, char); /* Locate character in a string */

/* history declaration*/
int build_histList(info_t *info, char *buff, int linecount); /* Build a history list */
int write_histFile(info_t *info); /* Write history to a file */
int renumberHist(info_t *info); /* Renumber history entries */
int read_histFile(info_t *info); /* Read history from a file */
char *retrieve_histFile(info_t *info); /* Get the history file path */

/* variable declaration */
void lookFor_chain(info_t *, char *, size_t *, size_t, size_t); /* Check and handle chaining */
int isChainDelimiter(info_t *, char *, size_t *); /* Check if there is a chaining operator */
int replaceAlias(info_t *); /* Replace aliases in the input */
int sub_strn(char **, char *); /* Replaces a string with a new one */
int replaceVariables(info_t *); /* replaces variable in the tokenized string */

/* string declaration */
int cmpstrn(const char *, const char *, size_t) __attribute__((unused)); /* Compare two strings */
int lenstrn(char *); /* Get the length of a string */
char *catstrn(char *, char *); /* Concatenate two strings */
char *starts_with_strn(const char *, const char *); /* Check if a string starts with another */

/* errors1 declaration*/
int err_atoi(char *); /* Convert a string to an integer with error handling */
void printErro(info_t *, char *); /* Print an error message */
int printDeci(int, int); /* Print a decimal number */
char *number_to_strn(long int, int, int); /* Convert a number to a string with formatting */
void remv_comnt(char *); /* Remove comments from a string */

/* atoi declaration*/
int handleInteract(info_t *); /* Handle interactive mode */
int check_delim(char, char *); /* Check if a character is a delimiter */
int check_alphabet(int); /* Check if a character is alphabetic */
int strn_to_integer(char *); /* Convert a string to an integer */

/* builtin declaration*/
int exit_cmd(info_t *); /* Built-in exit command */
int chnge_dir(info_t *); /* Built-in help command */
int cd(info_t *); /* Built-in cd command */

/* lists1 declaration*/
ssize_t get_node(list_t *, list_t *); /* Get the index of a node in a linked list */
char **list_to_strn(list_t *); /* Convert a linked list to an array of strings */
size_t prints_list(const list_t *); /* Print a linked list */
list_t *find_node(list_t *, char *, char); /* Find a node that starts with a given prefix */
size_t listLength(const list_t *); /* Get the length of a linked list */

/* tokenizer declaration*/
char **splitString1(char *, char); /* Tokenize a string with a single delimiter */
char **splitString(char *, char *); /* Tokenize a string */

/* getline declaration*/
void handle_sign(int); /* Handle SIGINT signal */
ssize_t inputGet(info_t *); /* Get user input */
int get_line(info_t *, char **, size_t *); /* Get a line of input */

/* environ declaration*/
char *get_env(info_t *, const char *); /* Get an environment variable */
int env_cmd(info_t *); /* Built-in env command */
int setenv_cmd(info_t *); /* Built-in setenv command */
int unsetenv_cmd(info_t *); /* Built-in unsetenv command */
int populateEnv_list(info_t *); /* Populate environment variables list */

/* shell loop declaration*/
int search_builtin(info_t *);/* Find built-in commands */
int hash(info_t *, char **); /* Hash function */
void forkExe_command(info_t *); /* Fork and execute a command */
void find_command(info_t *); /* Find a command */

/* lists declaration*/
size_t print_strList(const list_t *); /* Print a linked list of strings */
void freeList(list_t **); /* Free a linked list */
list_t *nodeadd_end(list_t **, const char *, int); /* Add a node to the end of a linked list */
int nodeDelete_at_index(list_t **, unsigned int); /* Delete a node at a specific index */
list_t *nodeadd(list_t **, const char *, int); /* Add a node to a linked list */

/* memory declaration*/
int memfree(void **); /* Free memory safely */

/* builtin1 declaration*/
int alias_cmd(info_t *); /* Built-in alias command */
int hist_cmd(info_t *); /* Built-in history command */

/* getenv declaration*/
char **getEnviron(info_t *); /* Get the environ array */
int _mysetenv(info_t *, char *, char *); /* Set an environment variable */
int _myunsetenv(info_t *, char *); /* Unset an environment variable */

/* parse declaration */
char *lookFor_path(info_t *, char *, char *); /* Find the executable path */
char *dup_chars(char *, int, int); /* Duplicate characters from a string */
int cmd_exists(info_t *, char *); /* Check if a command exists */

/* reallocation declaration */
void free_strn(char **); /* Free an array of strings */
void *realloc_mem(void *, unsigned int, unsigned int); /* Reallocate memory */
char *mem_set(char *, char, unsigned int); /* Fill memory with a constant byte */

/* get information declaration */
void infoSet(info_t *, char **); /* Set information in the info_t struct */
void infoClear(info_t *); /* Clear information in the info_t struct */
void infoFree(info_t *, int); /* Free resources in the info_t struct */

#endif /* MY_SHELL_H */
