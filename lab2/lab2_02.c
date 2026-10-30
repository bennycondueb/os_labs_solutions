#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>

/*
Exercise text taken from: https://www.skenz.it/os/lab02

A program receives two integer values ​​on the command line, called n and t. 
The program (parent process) must produce 2 children and terminate. 
In turn, each child must produce 2 children and terminate. 
This sequence of operations must continue until 2 ^ n processes on the leaves of the tree are produced/running. 
The leaf processes wait for t seconds and display (on screen) a termination message. 
Note that each process (in the tree) produces two other processes. 
Only those on the leaves of the tree sleep and display a message. 
What is the order of termination of the processes? Is it always the same? How can they be recognized (ppid)?

Once the program has been executed:

look for the pid of the various processes with the command ps
from the same shell
from another shell
terminate the execution of some processes with the kill command executed
from the same shel
from another shell
*/

int main(int argc, char* argv[]){

    // CLI parsing and variables declaration
    int n, t;
    const int NUM_CHILD = 2;

    if (argc != 3) {
		printf("Wrong number of arguments!\n");
	  	return 1;
    }

    n = atoi(argv[1]);
    t = atoi(argv[2]);

    // the whole exercise is done here
    for (int j = 0; j < n; j++) {
	  	fork();
    }


    sleep(t);
    printf("Hi I'm process %d, my parent is %d and I just terminated!\n", getpid(), getppid());

    return 0;
} 

/* Answers: 
	The order is non-deterministic, as it depends on the scheduler of my CPU. 
	Nevertheless, the first process, i.e. the one with parent my bash process, is always the first 
	one to be terminated. 
	We recognize the different processes thanks to getpid() and getppid()

	we can see that the parent process of the first process is the bash process
	how am I expected to kill processes that have already terminated?
*/