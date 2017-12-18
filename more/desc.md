### more01.c
- function:
    > implement a simple "more" cmd, show 24 lines then output a **more?** and wait user input.
- issue:
    > if we run "ls /bin | ./more01", then stdin was redirected, we will read user input in the same stream, which is "ls /bin", this is very terrible.  

### more02.c
- function:
    > Based on more01.c, this version resolve last version's issue. The solution is: read data which we want to show by stdin, then receive user input from keyboard. In Linux, there is a file named /dev/tty, which is a device description file belong keyboard and screen, write to this file equals print characters to screen, read from this file equals read user input from keyboard. In despite of whether stdin was redirected, we still can transfer data with terminal by this file.

### further
- how to get user input without Enter ?
- how to remove prompt **more?** ?
- how to show percent of input file ?
- how to determine default show line number ?

    [source code completely](http://bazaar.launchpad.net/~vcs-imports/util-linux-ng/trunk/view/head:/text-utils/more.c)