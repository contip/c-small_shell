#ifndef INPUT_H_
#define INPUT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* number of bytes allocated at a time for user input */
static const int CHUNK_SIZE = 128;

/* gets line of text from stdin, dynamically allocating memory
 * in blocks according to the CHUNK_SIZE constant */
char* get_line(void);

/* from line of text (assumed to be a line of shell input), parses line
 * to set background status, identify input/output redirection file names,
 * and expand the variable "$$" to the shell's pid.  Returns an array
 * consisting of the command as first element, and any parameters as
 * additional elements */
char** parse_line(char* line, int* num_arguments, int* background,
    char* in_file, char* out_file, int pid);

#endif
