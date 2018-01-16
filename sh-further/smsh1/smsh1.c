/* smsh1.c -- small shell version 1
 * frist really useful version after prompting shell
 * this one parses the command line into strings
 * uses fork, exec, wait, and ignores signals
 * compile: cc smsh1.c splitline.c execute.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "smsh.h"

#define DEL_PROMPT "> "

int main() {
    char* cmdline, * prompt, ** arglist; 
    int result;
    void setup();

    prompt = DEL_PROMPT;
    setup();

    while ((cmdline = next_cmd(prompt, stdin)) != NULL) {
        if ((arglist = splitline(cmdline)) != NULL) {
            result = execute(arglist);
            freelist(arglist);
        }
        free(cmdline);
    }
    return 0;
}

/* 
 * purpose: initialize shell
 * returns: nothing calls fatal() if trouble
 */
void setup() {
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void fatal(char* s1, char* s2, int n) {
    fprintf(stderr, "Error: %s, %s\n", s1, s2);
    exit(n);
}