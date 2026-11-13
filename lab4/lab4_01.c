/*

Exercise text taken from: https://www.skenz.it/os/lab04

A.
Write a C program that reads integer numbers and prints in standard output (stdout)

fprintf (stdout, ...);
the even numbers and in standard error (stderr)

fprintf (stderr, ...);
the odd numbers. The program must terminate after the introduction of the value 0.

Make use of redirection for:

read integer numbers from the file “inFile.txt”, instead of from standard input
write even numbers to the file “evenFile.txt”, instead of standard output
write the odd numbers to the file “oddFile.txt”, instead of standard error.

Notes: 
Below the basic C program. 
For the sake of easeness, the input file inFile.txt is formatted 
with 1 number per line

Here's the shell stuff (B part):

Use pipe and redirection on shell command to perform the following operations:

count the number of files/directories contained in your “home” directory (do not analyze sub-directories)

find /home/ -maxdepth 1 | tail -n +2 | wc -l  ->  
find everything, start from the 2 line (to skip oneself) and the cound the lines

after downloading the file lab01e01in.txt in a directory, print the total number of strings in the file.

mkdir ex01dir ; touch ex01dir/labex.txt; curl https://www.skenz.it/listing/os/lab/lab01e01in.txt > ex01dir/labex.txt; 
cat ex01dir/labex.txt | wc -w

count the number of active processes in the system who are owned by the user “root” (Suggestion: use the command man ps to analyze the option -U of the command).


*/

#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {

    char *INPUT_FILE = "stdin";
    char *OUTPUT_EVEN = "stdout";
    char *OUTPUT_ODD = "stderr";

    if (argc == 1) {
        int number = 0;
        INPUT_FILE =  "inFile.txt"; OUTPUT_EVEN = "evenFile.txt"; OUTPUT_ODD = "oddFile.txt"; 
        FILE *fin = fopen(INPUT_FILE, "r"); 
        FILE *foutEven = fopen(OUTPUT_EVEN, "w");
        FILE *foutOdd = fopen(OUTPUT_ODD, "w");

        if (fin == NULL || foutEven == NULL || foutOdd == NULL ) {
            fprintf(stderr, "TROUBLE\n");
            return 2;
        }

        fprintf(foutEven, "Even: ");
        while ( fscanf(fin, "%d\n", &number) != EOF) {
            if (number == 0) break;

            if (number % 2 == 0) {
                fprintf(foutEven, "%d ", number);
            }
        } 
        fprintf(foutEven, "\n");
        rewind(fin);
    
        fprintf(foutOdd, "Odd: ");
        while ( fscanf(fin, "%d\n", &number) != EOF) {
            if (number == 0) break;

            if (number % 2 != 0) {
                fprintf(foutOdd, "%d ", number);
            }
        } 
        fprintf(foutOdd, "\n");

        close(fin); close(foutEven); close(foutOdd);
    }
    else {
        fprintf(stdout, "Even: ");
        for (size_t i = 1; i < argc; i++) {
            if (argv[i] == 0) {
                break;
            }
            if (atoi(argv[i]) % 2 == 0) {
                fprintf(stdout, "%d ", atoi(argv[i]));
            }
        }
        fprintf(stderr, "\nOdd: ");
        for (size_t i = 1; i < argc; i++) {
            if (argv[i] == 0) {
                break;
            }
            if (atoi(argv[i]) % 2 != 0) {
                fprintf(stderr, "%d ", atoi(argv[i]));
            }
        }
        fprintf(stderr, "\n");
        fprintf(stdout, "\n");
    }


    
    return 0;
}