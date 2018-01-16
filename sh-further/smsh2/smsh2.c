/* smsh2.c -- small shell version 2
 * small shell that supports command line parsing
 * and if..then..else..fi logic(by calling process())
 * compile: cc smsh2.c splitline.c execute.c process.c controlflow.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "smsh.h"

#define DEL_PROMPT "> "

int main() {
    char* cmdline, * prompt, ** arglist; 
    int result, process(char**);
    void setup();

    prompt = DEL_PROMPT;
    setup();

    while ((cmdline = next_cmd(prompt, stdin)) != NULL) {
        if ((arglist = splitline(cmdline)) != NULL) {
            result = process(arglist);
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