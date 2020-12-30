#include "input.h"

/*
 * Function:  get_line
 * --------------------
 *  gets line of text from stdin; dynamically allocates memory in
 *  chunks of specified size, depending on length of input
 *
 *  returns: string containing the line of text
 */
char* get_line(void)
{
    char* line = NULL, *temp = NULL;
    int size = 0, index = 0, cur_char = EOF;
    /* read input by char, if EOF or newline encountered, null-terminate
     * the output string and return */
    while (cur_char != 0) 
    {
        cur_char = getc(stdin);
        if (cur_char == EOF || cur_char == '\n')
        {
            cur_char = 0;
        }
        /* if the size allotted <= the current index of the output string being
            built, allocate another portion of memory */
        if (size <= index)
        {
            size += CHUNK_SIZE;
            temp = realloc(line, size);
            line = temp;
        }
        line[index++] = cur_char;
    }
    return line;
}

/*
 * Function:  parse_line
 * --------------------
 *  parses input line, creating array of arguments; also sets background
 *  status and input / output redirection file names, and expands instances
 *  of "$$" with the pid of the shell process
 *
 *  line: (char*) the line read from stdin and to be parsed
 *  num_arguments: (int*) ptr to int holding number of elements in args array 
 *  background: (int*) ptr to int specifying background status of command
 *  in_file: (char*) string to store stdin redirection file, if any
 *  out_file: (char*) string to store stdout redirection file, if any
 *  pid: (int) pid of shell process, replaces the "$$" expansion variable
 *
 *  returns: malloced array of strings representing parsed command arguments
 */
char** parse_line(char* line, int* num_arguments, int* background,
        char* in_file, char* out_file, int pid)
{
    int array_index = 0, size = CHUNK_SIZE, args = 0, multiplier = 1;
    char* token;
    char** token_array = malloc(CHUNK_SIZE * sizeof(char*));

    token = strtok(line, " ");
    while (token != NULL)
    {
        if (*token == '#')
        {
            break;  /* if first char is '#', line is a comment and ignored */
        }
        /* check for input and output redirection characters */
        if (strcmp(token, "<") == 0)
        {
            token = strtok(NULL, " ");
            strcpy(in_file, token);
            token = strtok(NULL, " ");
            continue;
        }
        else if (strcmp(token, ">") == 0)
        {
            token = strtok(NULL, " ");
            strcpy(out_file, token);
            token = strtok(NULL, " ");
            continue;
        }
        else if (strcmp(token, "&") == 0)
        {
            /* set background mode if last parameter is '&' */
            *background = 1;
            break;
        }
        else if (strstr(token, "$$") != NULL)
        {
            /* check for "$$" expansion variable (always appears alone
             * or at the end of an argument) */
            int token_len = strlen(token);
            token[token_len - 2] = '\0';
            char pid_str[20];  /* assumed len(pid) < 20 */
            sprintf(pid_str, "%d", pid);
            char* new_token = calloc(token_len + 20, sizeof(char));
            strcpy(new_token, token);
            strcat(new_token, pid_str);
            token_array[array_index] = strdup(new_token);
            free(new_token);
            args++;
            array_index++;
            token = strtok(NULL, " ");
            /* resize the args array if necessary */
            if (array_index >= (CHUNK_SIZE * multiplier))
            {
                size += CHUNK_SIZE;
                token_array = realloc(token_array, size * sizeof(char*));
                multiplier++;
            }
            continue;
        }
        else
        {
            /* if none of above cases, add token to the arguments array */
            token_array[array_index] = strdup(token);
            args++;
            array_index++;
            /* resize the args array if necessary */
            if (array_index >= (CHUNK_SIZE * multiplier))
            {
                size += CHUNK_SIZE;
                token_array = realloc(token_array, size * sizeof(char*));
                multiplier++;
            }
            token = strtok(NULL, " ");
        }
    }
    /* null-terminate the args array for use with execvp and return */
    token_array[array_index] = NULL;
    array_index = 0;
    *num_arguments = args;
    return token_array;
}

