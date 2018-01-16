/* hello3.c
 * purpose: using refresh and sleep for animated effects
 * outline: initialize, draw stuff, wrap up
 */

#include <stdio.h>
#include <curses.h>

int main() {
    int i;
    
    initscr();
    clear();
    for (i = 0; i < LINES; i++) {
        move(i, i+1);
        if (i % 2 == 1) {
            standout();
        }
        addstr("hello, world");
        if (i % 2 == 1) {
            standend();
        }
        sleep(1);
        refresh();
    }
    endwin();
}