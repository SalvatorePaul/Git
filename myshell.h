#ifndef MYSHELL_H
#define MYSHELL_H

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

#define BUFFER_FLUSH -1
#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024

#define CMND_CHAIN 3
#define CMND_AND 2
#define CMND_OR 1
#define CMND_NORM 0

#define CONVERTING_UNSIGNED 2
#define CONVERTING_LOWERCASE 1

#define USING_STRTOK 0
#define USING_GETLINE 0

#define FILE_HISTORY ".simple_shell_history"
#define MAX_HISTORY 4096

extern char **environ;

/**
 * struct liststr - A singly linked list node for storing strings
 * @str: The string data stored in the node.
 * @num: An optional integer value associated with the node
 * @next: A pointer to the next node in the linked list.
 */
typedef struct liststr
{
	char *str;/* String data stored in the node */
	int num;/* Integer data stored in the node (optional) */
	struct liststr *next;/* Pointer to the next node in the list */
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

#define INFO_INIT {NULL, NULL, NULL, NULL, NULL, NULL, NULL, \
	NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, 0, 0, 0}

/**
 *struct builtin - stores a built-in command string and its function pointer.
 *
 *@type: Pointer to the built-in command string.
 *@func: Pointer to the function handling the built-in command.
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

int loophsh(char **);

int _errputchar(char);
int _putsfnd(char *str, int fd);
int _putfnd(char c, int fd);
void _errputs(char *);

char *dupStrn(const char *);
int wrt_chr(char);
char *cpyStrn(char *, char *);
void prnt_strn(char *);

char *strn_cat(char *, char *, int);
char *strn_cpy(char *, char *, int);
char *str_chr(char *, char);

int build_histList(info_t *info, char *buff, int linecount);
int write_histFile(info_t *info);
int renumberHist(info_t *info);
int read_histFile(info_t *info);
char *retrieve_histFile(info_t *info);

void lookFor_chain(info_t *, char *, size_t *, size_t, size_t);
int isChainDelimiter(info_t *, char *, size_t *);
int replaceAlias(info_t *);
int sub_strn(char **, char *);
int replaceVariables(info_t *);

int cmpstrn(const char *, const char *, size_t) __attribute__((unused));
int lenstrn(char *);
char *catstrn(char *, char *);
char *starts_with_strn(const char *, const char *);

int err_atoi(char *);
void printErro(info_t *, char *);
int printDeci(int, int);
char *number_to_strn(long int, int, int);
void remv_comnt(char *);

int handleInteract(info_t *);
int check_delim(char, char *);
int check_alphabet(int);
int strn_to_integer(char *);

int exit_cmd(info_t *);
int chnge_dir(info_t *);
int cd(info_t *);

ssize_t get_node(list_t *, list_t *);
char **list_to_strn(list_t *);
size_t prints_list(const list_t *);
list_t *find_node(list_t *, char *, char);
size_t listLength(const list_t *);

char **splitString1(char *, char);
char **splitString(char *, char *);

void handle_sign(int);
ssize_t inputGet(info_t *);
int get_line(info_t *, char **, size_t *);

int search_builtin(info_t *);
int hash(info_t *, char **);
void forkExe_command(info_t *);
void find_command(info_t *);

size_t print_strList(const list_t *);
void freeList(list_t **);
list_t *nodeadd_end(list_t **, const char *, int);
int nodeDelete_at_index(list_t **, unsigned int);
list_t *nodeadd(list_t **, const char *, int);

int memfree(void **);

int alias_cmd(info_t *);
int hist_cmd(info_t *);

char **getEnviron(info_t *);
int _mysetenv(info_t *, char *, char *);
int _myunsetenv(info_t *, char *);

char *lookFor_path(info_t *, char *, char *);
char *dup_chars(char *, int, int);
int cmd_exists(info_t *, char *);

void free_strn(char **);
void *realloc_mem(void *, unsigned int, unsigned int);
char *mem_set(char *, char, unsigned int);

void infoSet(info_t *, char **);
void infoClear(info_t *);
void infoFree(info_t *, int);

#endif /* MY_SHELL_H */
