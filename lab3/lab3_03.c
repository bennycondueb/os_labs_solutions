/*

Exercise Text taken from: https://www.skenz.it/os/lab03
Also "commands.txt" taken from: https://www.skenz.it/os/lab03 !!


A file contains strings logically grouped into distinct sets. 
Any set of strings indicates a Unix/Linux command and the related parameters. 
Each set is terminated with the string “end”. The following is an example of correct file:

touch mioFile.c end
ls -laR end
cat mioFile.c end
mkdir -p tmpDir end
cp -f mioFile.c tmpDir end

Observe that, if you prefer, you can assume that the string “end” is not present in the file, 
and it can be substituted with the end-of-line character \n to indicate the end of the strings associated with a certain command.

Write a program that, after receiving the name of the file through the command line, executes the Unix/Linux command listed in the file by using:

the system call system, executing one command after another
the system call exec, executing a command every 3 seconds (you can decide with version of the exec to use).

In this solution, we will use the assumption there is no "end" at the end of the line, 
just the "\n" character. 
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

const size_t ARGUMENTS_NUM = 2;
const size_t BUFFER_SIZE = 100;

int main(int argc, char* argv[]) {

  if (argc != ARGUMENTS_NUM) {
    fprintf(stderr, "Wrong number of arguments!!\n");
    return 1;
  }

  FILE *finput = fopen(argv[1], "r");

  if (finput == NULL) {
    fprintf(stderr, "File doesn't exist!!\n");
    return 1;
  }

  char *buffer = (char *) malloc(sizeof(char) * BUFFER_SIZE);

  if (buffer == NULL) {
    fprintf(stderr, "MALLOC() failed!!\n");
    return 1;
  }

  char *command_to_exec, *arguments_to_parse; 
  size_t length_command_to_exec = 0, length_buffer = 0;
  bool useSystem = false;

  // we read the whole file line by line
  while ( (length_buffer = getline(&buffer, &BUFFER_SIZE, finput)) != -1 ) {
    length_command_to_exec = 0;

    for (size_t i = 0; i < BUFFER_SIZE && buffer[i] != '\0' && !length_command_to_exec; i++) {
      if (buffer[i] == ' ') 
        length_command_to_exec = i + 1; 
    }

    length_command_to_exec--;

    if (useSystem) {
      system(buffer);
    }
    else {
      size_t number_of_args = 0, length_arguments_to_parse = length_buffer - length_command_to_exec - 1;
      size_t dummy = 0, index_args = 0;
      command_to_exec = (char*) malloc( sizeof(char) * length_command_to_exec); 
      arguments_to_parse = (char*) malloc(sizeof(char) * (length_arguments_to_parse));

      if (command_to_exec == NULL || arguments_to_parse == NULL) {
        fprintf(stderr, "MALLOC() failed!!\n");
        return 1;
      }

      for (size_t i = 0; i < length_command_to_exec; i++) {
        command_to_exec[i] = buffer[i];
      }

      for (size_t i = 0; i < length_arguments_to_parse; i++) {
        arguments_to_parse[i] = buffer[i + length_command_to_exec];
      }

      for (size_t i = 0; i < length_arguments_to_parse; i++)
        if (arguments_to_parse[i] == ' ') number_of_args++;

      number_of_args++;
      number_of_args++;

      char **args = (char**) malloc(sizeof(char *) * number_of_args);

      for (size_t i = 0; i < length_arguments_to_parse; i++) {
        if (arguments_to_parse[i] == ' ' || arguments_to_parse[i] == '\n') {
          args[index_args] = (char *) malloc( sizeof(char) * (dummy));

          if (args[index_args] == NULL) {
            fprintf(stderr, "MALLOC FAILED!!\n");
            return 1;
          }

          for (int j = dummy; j >= 0; j--) {
            args[index_args][dummy - j] = arguments_to_parse[i - j];
          }

          index_args++;
          dummy = 0;
        }
        else {
          dummy++;
        }
      }

      args[number_of_args - 1] = NULL;
      for (int i = 0; i < number_of_args; i++) 
        printf("%s \n", args[i]);

      if (!fork()) {
        execv(command_to_exec, args);
      }

    }

  }
  


  return 0;
}