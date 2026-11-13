/*
Exercise text taken from: https://www.skenz.it/os/lab04

Exercise 02: Concurrent exchange-sort on files
Premise number 1.
The sorting algorithms by exchange (exchange-sort or bubble-sort) sort a vector comparing adjacent elements. 
The following is a possible implementation (with sorting of the vector v of n elements):

for (i=0; i<n-1; i++) {
  for (j=0; j<n-1-i; j++) {
    if (v[j] > v[j+1]) {
       val = v[j];
       v[j] = v[j+1];
       v[j+1] = val;
    }
  }
}

Premise number 2.
A binary file can be write (UNIX function write) or read (UNIX function read) using a random access by means of the function lseek.

For example, the program lab04ex02.c, after receiving the name of a file from command line,

it writes in the file the vector vet with 12 elements
it reads the file starting from the last line.


Exercise specifications
Implement in a concurrent and multi-process way the bubble-sort algorithm.

The algorithm, by using the system calls fork and wait, and sharing data in the file 
(i.e., by reading and writing data in the same file), must order the file itself.

The program (i.e., the parent):

receives on the command line the name of a file containing a predefined number n of integer numbers.
at each iteration of the external cycle, it “simulates” the action of the internal cycle by using n-1-i child processes
Each of its children:

reads two adjacent elements from the file
swaps them if it is necessary
writes them in the file
it synchronizes with the other children processes to generate the overall sorting effect 
(i.e., at each iteration in the external cycle the highest value among the remaining numbers is moved at the end of the file)
terminates

Observations

Working on file is indispensable since a parent process and a child do not share the address space 
(in fact, ordering a vector would actually order different vectors).
The program can be generalized by considering the first integer number stored in the file as indicator of 
the actual number of the integer numbers saved in the file.
Managing a binary file simplifies the positioning on a specific data i.
In fact, in the case of a ASCII file, the number of character occupied is not constant, 
as in the case of binary file, but it depends on the value of the number:

number 5
ASCII coding = 1 character
binary coding = sizeof(int) bytes,

number 12345
ASCII coding = 5 characters
binary coding = sizeof(int) bytes,

In addition, in the first case of ASCII coding, it would also be necessary to count “additional” characters, 
such as the \n character of end of line.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main( int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "TROUBLE\n");
        return 1;
    }

    int fdesc = open(argv[1], O_RDWR);
    int number = 0, tot_nums = 0;
    size_t advance = 0;

    if (fdesc == -1) {
        fprintf(stderr, "TROUBLE\n");
        return 1;
    }

    if (read(fdesc, &tot_nums, sizeof(int)) == -1) {
        fprintf(stderr, "TROUBLE\n");
        return 1;
    }

    fprintf(stdout, "%d \n", tot_nums);

    //for (size_t i = 0; i < tot_nums; i++) {
    //    advance = read(fdesc, &number, sizeof(int));
    //    fprintf(stdout, "%d \n", number);
    //}




    close(fdesc);
} 