#include <stdio.h> 
#include <stdlib.h> 
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

/*
Exercise text taken from: https://www.skenz.it/os/lab02

Using the system calls fork and wait, realize the following precedence graph.

        P1
       /  \ 
      /    \ 
    P2      P3
    /\      /\
   /  \    /  \
 P4   P5  |   P6
   \  /    \  /
    \/      \/
    P7      P8
      \    /
       \  / 
        P9

Note that all arcs are oriented downwards. Each Pi process corresponds to a print message (print the string “Pi” and the PID of the process).

Check the precedence are respected by inserting the system call sleep in the various branches of the program.
*/

/*
Benny's note: the use of wait instead of waitpid 
is just to use easier and simpler functions w.r.t. waitpid
*/

int main(int argc, char *argv[]) {

    // variables declaration
    pid_t pid_1, pid_2, pid_3;
    int *wstatus = malloc(sizeof(int*));


    printf("P1\n");
    pid_1 = fork();

    // child process
    if (pid_1 == 0) {

        // Hey I'm P3! 
        printf("P3\n");
        pid_3 = fork(); 

        // Create P6 and do something then finish! 
        if (pid_3 == 0) {
            printf("P6\n");
            exit(0);
        }
        
        // sleep necessary to put everything in order
        // ( as requested from text )
        sleep(1);
        printf("P8\n");
        exit(0);
    }
    // parent process! oh yeah!
    else {
        printf("P2\n");
        pid_2 = fork();

        // again, child, we are P5!
        if (pid_2 == 0) {
            wait((int *) 0);
            printf("P5\n");

            // finished P5, we exit so we can join! 
            exit(0);
        }
        // parent stays P4! 
        else {
            printf("P4\n");
        }

        // sleep necessary to put everything in order
        // ( as requested from text )
        sleep(1); 
        wait((int *)0);
        printf("P7\n");
        
    }

    wait((int*) 0);
    printf("P9\n");

    return 0;
}