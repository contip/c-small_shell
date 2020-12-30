#ifndef BUILT_INS_H
#define BUILT_INS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

static const char* BUILT_INS[] = { "exit", "cd", "status" };
static const int NUM_BUILT_INS = 3;
/* functions provided */
/* prints the exit or termination status associated with the input
 * exit_status variable */
void print_exit(int exit_status);
/* exits the shell */
int exit_shell(char** args, int num_args, int* exit_status);
/* changes current working directory to the specified path, or to
 * the user's home directory if no path given */
int change_dir(char** args, int num_args, int* exit_status);
/* gets the exit status of the last completed non-built-in command */
int get_status(char** args, int num_args, int* exit_status);
#endif
