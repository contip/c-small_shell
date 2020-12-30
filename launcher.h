#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <signal.h>
#include <string.h>
#include "built_ins.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static const char* FG_MODE_MSG = "\nNow entering foreground-only mode "
                                 "(& is ignored)!\n\0";
static const char* FG_MODE_OFF_MSG = "\nExiting foreground-only mode!\n\0";

/* SIGTSP signal handler (toggles fg-only mode on and off */
void handle_SIGTSTP(int signo);
/* shell launcher, attempts to execute the last line of shell input */
int shell_exec(char** args, int num_args, int* exit_status, int* background,
               char* in_file, char* out_file, struct sigaction* ignore_action);

#endif
