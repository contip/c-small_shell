#include "built_ins.h"

/*
 * Function:  print_exit
 * --------------------
 *  prints the exit / termination info associated with given exit status
 *
 *  exit_status: (int) holds exit status of last run child process
 *
 *  returns: none, prints results to screen
 */
void print_exit(int exit_status)
{
    if (WIFEXITED(exit_status))
    {
        printf("exit value %d\n", WEXITSTATUS(exit_status));
    }
    else
    {
        printf("terminated by signal %d\n", WTERMSIG(exit_status));
    }
}

/*
 * Function:  exit_shell
 * --------------------
 *  exits from the shell 
 *
 *  note: parameters unused, but included for function pointer array
 *
 *  returns: integer 0 (terminates the main shell loop)
 */
int exit_shell(char** args, int num_args, int* exit_status)
{
    return 0;
}

/*
 * Function:  change_dir
 * --------------------
 *  changes the current working directory to the absolute or relative path
 *  specified in the args parameter args[1]
 *  if args[1] does not exist, changes cwd to user's home directory
 *
 *  args: (char**) array of strings representing the command and parameters
 *  num_args: (int) the number of elements in the args array
 *  exit_status: (int*) UNUSED, included for function pointer array
 *
 *  returns: integer 1 (continues the main shell loop)
 */
int change_dir(char** args, int num_args, int* exit_status)
{
    if (num_args > 2)
    {
        /* the cd command takes 1 parameter, if more are given, invalid */
        perror("cd failed -- invalid syntax (too many arguments)");
    }
    else if (!args[1])
    {
        /* if no parameter is given with cd command, cd to user's home dir */
        if (chdir(getenv("HOME")) != 0)
        {
            perror("cd command failed!\n");
        }
    }
    else
    {
        /* otherwise, cd to the specified absolute or relative path */
        if (chdir(args[1]) != 0)
        {
            perror("cd command failed!\n");
        }
    }
    return 1;
}

/*
 * Function:  get_status
 * --------------------
 *  gets the exit status associated with the last-run non-built-in command
 *
 *  args: (char**) UNUSED, included for function pointer array
 *  num_args: (int) UNUSED, included for function pointer array
 *  exit_status: (int*) ptr to int tracking the exit status of child processes
 *
 *  returns: integer 1 (continues the main shell loop)
 */
int get_status(char** args, int num_args, int* exit_status)
{
    print_exit(*exit_status);
    return 1;
}

