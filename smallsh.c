#include "input.h"
#include "built_ins.h"
#include "launcher.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("welcome in smallsh (pid %d)\n", getpid());
    char* line;
    char** args;
    char in_file[255] = { 0 }, out_file[255] = { 0 };
    int i, num_args, exec_result, exit_status = 0, background = 0,
        pid = getpid();
    struct sigaction SIGTSTP_action = { 0 }, ignore_action = { 0 };

    /* SIGTSTP signal handling (toggle fg-only mode) */
    SIGTSTP_action.sa_handler = handle_SIGTSTP;
    /* block catchable signals while the handler is running */
    sigfillset(&SIGTSTP_action.sa_mask);
    /* make sure no sigaction flags are set */
    SIGTSTP_action.sa_flags = 0;
    /* add custom SIGTSTP handler to the sigaction struct */
    sigaction(SIGTSTP, &SIGTSTP_action, NULL);

    /* SIGINT signal handling (ignored by shell process) */
    ignore_action.sa_handler = SIG_IGN;
    sigfillset(&ignore_action.sa_mask);
    ignore_action.sa_flags = 0;
    sigaction(SIGINT, &ignore_action, NULL);

    do  /* main shell loop */
    {
        /* reset background, in_file, and out_file with each iteration */
        background = 0;
        memset(in_file, 0, sizeof(in_file));
        memset(out_file, 0, sizeof(out_file));
        printf(": ");  /* user prompt */
        fflush(stdout);
        line = get_line();
        args = parse_line(line, &num_args, &background,
                in_file, out_file, pid);
        exec_result = shell_exec(args, num_args, &exit_status, &background,
                      in_file, out_file, &ignore_action);
        /* free memory */
        for (i = 0; i < num_args; i++)
        {
            free(args[i]);
        }
        free(args);
        free(line);
    }
    while (exec_result);
    return 0;
}

