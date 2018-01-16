/* ticker_demo.c
 *      demonstrates use of interval timer to generate reqular
 *      signals, which are in turn caught and used to count down
 * compile cc ticker_demo.c set_ticker.c
 */

#include <stdio.h>
#include <sys/time.h>
#include <signal.h>

int set_ticker(int);

int main() {
    void countdown(int);

    signal(SIGALRM, countdown);
    if (set_ticker(500) == -1) {
        perror("set_ticker");
    } else {
        while (1) {
            pause();
        }
        return 0;
    }
}

void countdown(int signum) {
    static int num = 10;
    printf("%d...", num--);
    fflush(stdout);
    if (num < 0) {
        printf("DONE!\n");
        exit(0);
    }
}
