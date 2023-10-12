#include "myshell.h"

/**
 * handle_sign - Signal handler for blocking Ctrl-C (SIGINT).
 * @sig_num: The signal number (unused but required by the signal handler).
 * This function is a signal handler for blocking Ctrl-C signals. It is
 * typically used to handle Ctrl-C interruptions gracefully, allowing the
 * program to continue after displaying a prompt.
 * Return: void
 */
void handle_sign(__attribute__((unused))int sig_num)
{
	/* Print a newline character and a prompt */
	prnt_strn("\n");
	prnt_strn("$ ");
	/* Flush the output buffer */
	wrt_chr(BUFFER_FLUSH);
}

/**
 * input_buf - Buffer chained commands and read input.
 * @info: Parameter struct containing information about the input.
 * @buf: Pointer to the buffer for storing input data (updated by the function).
 * @len:  Pointer to the length of the data in the buffer (updated by the function).
 * This function is responsible for buffering chained commands and reading
 * input from the standard input (stdin) into the provided buffer (buf).
 * It updates the 'buf' and 'len' pointers to store the read data.
 *
 * Return: The number of bytes read, or 0 if the buffer is not empty,
 *         or -1 on error.
 */
ssize_t input_buf(info_t *info, char **buff, size_t *len)
{
        size_t len_p = 0;
        ssize_t a = 0;

        /* If the buffer is empty, fill it with input data */
        if (!*len)
        {
                /* Free the previous buffer and set it to NULL */
                free(*buff);
                *buff = NULL;
                /* Set up a signal handler for Ctrl-C interruptions */
                signal(SIGINT, handle_sign);
#if USE_GETLINE

                /* Use getline to read input into the buffer */
                a = getline(buff, &len_p, stdin);
#else
                /* Use a custom _getline function to read input */
                a = get_line(info, buff, &len_p);
#endif
                if (a > 0)
                {
                        if ((*buff)[a - 1] == '\n')
                        {
                                /* Remove trailing newline if present */
                                (*buff)[a - 1] = '\0'; /* remove trailing newline */
                                a--;
                        }
                        /* Set the linecount_flag and process the input */
                        info->linecount_flag = 1;
                        remv_comnt(*buff);
                        build_histList(info, *buff, info->historycount++);
                        /* Check if this is a command chain (';'-separated commands) */
                        {
                                *len = a;
                                info->cmd_buffer = buff;
                        }
                }
        }
	/* Return the number of bytes read (r) or -1 on error
    return (r); */
        return (a);
}
/**
 * inputGet - Read a line of input, excluding the newline character.
 * @info: Parameter struct containing input information.
 *
 * Return: The number of bytes read.
 */
ssize_t inputGet(info_t *info)
{
	ssize_t a = 0;
	static size_t b;
	static size_t j;
	static size_t c;
	char **buff_p = &(info->arg), *p;
	static size_t len;
	static char *buff; /* the ';' command chain buffer */
	
	wrt_chr(BUFFER_FLUSH);
	/* Read input into the buffer and track the number of bytes read */
	a = input_buf(info, &buff, &len);
	if (a == -1) /* EOF */
		return (-1);
	if (len)	/* we have commands left in the chain buffer */
	{
		c = b; /* init new iterator to current buf position */
		p = buff + b; /* get pointer for return */

		lookFor_chain(info, buff, &j, b, len);
		while (c < len) /* iterate to semicolon or end */
		{
			if (isChainDelimiter(info, buff, &c))
				break;
			c++;
		}

		b = c + 1; /* increment past nulled ';'' */
		if (b >= len) /* reached end of buffer? */
		{
			b = len = 0; /* reset position and length */
			info->cmd_buffer_type = CMND_NORM;
		}

		*buff_p = p; /* pass back pointer to current command position */
		return (lenstrn(p)); /* return length of current command */
	}

	*buff_p = buff; /* else not a chain, pass back buffer from _getline() */
	return (a); /* return length of buffer from _getline() */
}

/**
 * read_buf - Read data into a buffer.
 * @info: Parameter struct containing information about the read operation.
 * @buf: The buffer to store the read data.
 * @i: Pointer to the size of the data read (updated by the function).
 * This function reads data from a file descriptor (info->readfd) into the
 * provided buffer (buf) up to a specified buffer size (READ_BUF_SIZE).
 * It updates the 'i' pointer with the number of bytes read.
 *
 * Return: The number of bytes read (r), or 0 if the buffer is not empty,
 *         or -1 on error.
 */
ssize_t read_buf(info_t *info, char *buf, size_t *i)
{
	ssize_t r = 0;

	/* If the buffer is not empty, return 0 (no data read) */
	if (*i)
		return (0);
	/* Read data into the buffer from the specified file descriptor */
	r = read(info->readfd, buf, READ_BUFFER_SIZE);
	/* Update the 'i' pointer with the number of bytes read */
	if (r >= 0)
		*i = r;
	/* Return the number of bytes read (r) or -1 on error */
	return (r);
}

/**
 * get_line - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int get_line(info_t *info, char **ptr, size_t *length)
{
        static size_t a;        /* Static position indicator */
        ssize_t b = 0; /* read status variables */
        ssize_t s = 0;  /* Size status variables */
        size_t c;       /* Temporary size variable */
        char *p = NULL; /* Input pointer */
        char *new_p = NULL;     /* New pointer for reallocation */
        char *pnl;      /* Pointer to newline character */
        static size_t leng;     /* Length of the read buffer */
        static char buff[READ_BUFFER_SIZE]; /* Read buffer */

        p = *ptr;       /* Set input pointer */
        if (p && length)
        s = *length;    /* Update length if provided */
        if (a == leng)
        a = leng = 0;   /* Reset position indicators */
        b = read_buf(info, buff, &leng); /* Read data */
        if (b == -1 || (b == 0 && leng == 0))
        return (-1);    /* Check for errors */
        pnl = str_chr(buff + a, '\n'); /* Find newline */
	c = pnl ? 1 + (unsigned int)(pnl - buff) : leng; /* Calculate size */
        new_p = realloc_mem(p, s, s ? s + c : c + 1); /* Reallocate memory */
        if (!new_p)
        return (p ? free(p), -1 : -1); /* Check memory allocation */
        if (s)
        strncat(new_p, buff + a, c - a); /* Concatenate strings */
        else
        strncpy(new_p, buff + a, c - a + 1); /* Copy strings */
        s += c - a;     /* Update size */
        a = c;  /* Update position */
        p = new_p;      /* Update pointer */
        if (length)
        *length = s;    /* Update length if provided */
        *ptr = p;       /* Update pointer */

        return (s);     /* Return the size */
}
