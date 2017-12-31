/* play_again2.c
 * purpose: ask if user wants another transaction
 * method: set tty into char-by-char mode and no-echo mode
 *         read char, return result
 * returns: 0 => yes, 1 => no
 * better: timeout if user walks away
 */

#include <stdio.h>
#include <termios.h>

#define QUESTION "Do you want another transaction"

void set_cr_noecho_mode();
int get_response(char*);
int tty_mode(int);

int main() {
    int response;
    tty_mode(0); // save mode
    set_cr_noecho_mode(); // set -icanon, -echo
    response = get_response(QUESTION); // get some answer
    tty_mode(1); // restore tty state
    return response;
}

/*
 * purpose: ask a question and wait for a y/n answer
 * method: use getchar and ignore non y/n answers
 * returns: 0 => yes, 1 => no
 */
int get_response(char* question) {
    printf("%s (y/n)?", question);
    while (1) {
        switch (getchar()) {
            case 'y':
            case 'Y': return 0;
            case 'n':
            case 'N':
            case EOF: return 1;
        }
    }
}

/*
 * purpose: put file descriptro 0 into char-by-char mode and noecho mode
 * method: use bits in termios
 */ 
void set_cr_noecho_mode() {
    struct termios ttystate;
    tcgetattr(0, &ttystate); // read curr setting
    ttystate.c_lflag &= ~ICANON; // no buffering
    ttystate.c_lflag &= ~ECHO; // no echo either
    ttystate.c_cc[VMIN] = 1; // get 1 char at a time
    tcsetattr(0, TCSANOW, &ttystate); // install settings
}

// how == 0 => save current mode, how == 1 => restore mode
int tty_mode(int how) {
    static struct termios original_mode;
    if (how == 0) {
        return tcgetattr(0, &original_mode);
    } else {
        return tcsetattr(0, TCSANOW, &original_mode);
    }
}