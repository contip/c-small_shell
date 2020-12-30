#include "launcher.h"

/* array of function ptrs for the built-in commands */
int (*built_in_funcs[]) (char**, int, int*) = { &exit_shell, &change_dir,
                                                &get_status };
/* global fg_only variable (can not pass params to signal handler) */
int fg_only = 0;

/*
 * Function:  handle_SIGTSTP
 * --------------------
 *  custom handler function for the SIGTSTP (^Z) signal 
 *  toggles the program's foreground-only mode on / off
 *
 *  returns: none 
 */
void handle_SIGTSTP(int signo)
{
    if (fg_only == 0)
    {
        write(STDOUT_FILENO, FG_MODE_MSG, strlen(FG_MODE_MSG));
        fflush(stdout);
        fg_only = 1;
    }
    else
    {
        write(STDOUT_FILENO, FG_MODE_OFF_MSG, strlen(FG_MODE_OFF_MSG));
        fflush(stdout);
        fg_only = 0;
    }

}

/*
 * Function:  shell_exec
 * --------------------
 *  launcher function used by shell to execute built-in and custom commands
 *
 *  args: (char**) array containing the command and its parameters, if any
 *  num_args: (int) number of elements in args array
 *  exit_status: (int*) ptr to int tracking the exit status of child processes
 *  background: (int*) ptr to int tracking background status of commands
 *  in_file: (char*) path to file used for stdin redirection, if any
 *  out_file: (char*) path to file used for stdout redirection, if any
 *  ignore_action: (struct sigaction*) ptr to the ignore_action sa struct,
 *                  which handles the SIGINT signal 
 *
 *  returns: integer 1 (continues the main shell loop)
 */
int shell_exec(char** args, int num_args, int* exit_status, int* background,
               char* in_file, char* out_file, struct sigaction* ignore_action)
{
    int i, in_FD, out_FD, result;
    if (args[0] == NULL)
    {
        return 1;
    }

    /* if the command is one of the built-ins, call appropriate function */
    for (i = 0; i < NUM_BUILT_INS; i++)
    {
        if (strcmp(args[0], BUILT_INS[i]) == 0)
        {
            return (*built_in_funcs[i])(args, num_args, exit_status);
        }
    }

    /* if command not in built-ins, fork a child process */
    pid_t spawn_pid = fork();
    switch (spawn_pid)
    {
        case -1:
            perror("fork() failed!\n");
            exit(1);
            break;
        case 0:  /* code in case 0 is executed by child */
            /* child processes must not ignore CTRL-C (SIGINT) */
            ignore_action->sa_handler = SIG_DFL;  /* set handler to default */
            sigaction(SIGINT, ignore_action, NULL);  /* add to struct */

            /* check for stdin redirection */
            if (strcmp(in_file, ""))
            {
                in_FD = open(in_file, O_RDONLY);
                if (in_FD == -1)
                {
                    perror("Error opening input file!\n");
                    exit(1);
                }
                /* redirect stdin to the input file descriptor using dup2 */
                result = dup2(in_FD, STDIN_FILENO);
                if (result == -1)
                {
                    perror("error redirecting stdin with dup2!\n");
                    exit(2);
                }
                close(in_FD);  /* close fd now that stdin redirected to it */
            }

            /* check for stdout redirection */
            if (strcmp(out_file, ""))
            {
                /* open the specified target file */
                out_FD = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (out_FD == -1)
                {
                    perror("Error opening output file!\n");
                    exit(1);
                }
                /* redirect to stdout */
                result = dup2(out_FD, STDOUT_FILENO);
                if (result == -1)
                {
                    perror("error redirecting stdout with dup2!\n");
                    exit(2);
                }
                close(out_FD);
            }

            /* attempt to execute the command with its parameters */
            result = execvp(args[0], args);
            if (result == -1)
            {
                printf("error executing command: %s\n", args[0]);
                fflush(stdout);
                exit(1);
            }
            break;
        default:  /* code in default is executed by parent */
            /* if args contained '&' and not fg_only mode, run in background */
            if (*background && !fg_only)
            {
                waitpid(spawn_pid, exit_status, WNOHANG);
                printf("the background pid is %d\n", spawn_pid);
                fflush(stdout);
            }
            /* otherwise, run in foreground */
            else
            {
                waitpid(spawn_pid, exit_status, 0);
            }
    }

    while ((spawn_pid = waitpid(-1, exit_status, WNOHANG)) > 0)
    {
        printf("child %d has been terminated\n", spawn_pid);
        print_exit(*exit_status);
        fflush(stdin);
    }
    return 1;
}

