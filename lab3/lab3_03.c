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
#include <stdbool.h>
#include <sys/wait.h>

const size_t ARGUMENTS_NUM = 2;
const size_t BUFFER_SIZE = 100;

void argument_parser(char *buffer, 
                     char *command_to_exec, 
                     char *arguments_to_parse, 
                     size_t length_command_to_exec, 
                     size_t *number_of_args);

void args_matrix_creation(char **args, 
                          char *command_to_exec,
                          char *arguments_to_parse,
                          size_t number_of_args);


int main(int argc, char* argv[]) {

  // Variable creation & Malloc checks
  FILE *finput = fopen(argv[1], "r");
  char *buffer = (char *) malloc(sizeof(char) * BUFFER_SIZE);
  char *command_to_exec, *arguments_to_parse; 
  size_t length_command_to_exec = 0, length_buffer = 0;
  bool useSystem = false;

  if (argc != ARGUMENTS_NUM) {
    fprintf(stderr, "Wrong number of arguments!!\n");
    return 1;
  }
  if (finput == NULL || buffer == NULL) {
    fprintf(stderr, "File doesn't exist!! or MALLOC fail!!\n");
    return 1;
  }

  // we read the whole file line by line
  while ( (length_buffer = getline(&buffer, &BUFFER_SIZE, finput)) != -1 ) {
    
    // parsing the command to exec
    length_command_to_exec = 0;
    for (size_t i = 0; i < BUFFER_SIZE && buffer[i] != '\0' && !length_command_to_exec; i++) {
      if (buffer[i] == ' ') 
        length_command_to_exec = i + 1; 
    }
    length_command_to_exec--;

    // if we use the system system call it's enough the buffer
    if (useSystem) {
      system(buffer);
    }

    // instead if we use the exec system call we are required to do more steps
    else {

      // variable declaration and MALLOC checks
      size_t number_of_args = 2, length_arguments_to_parse = length_buffer - length_command_to_exec;
      command_to_exec = (char*) malloc( sizeof(char) * length_command_to_exec + 1); 
      arguments_to_parse = (char*) malloc(sizeof(char) * (length_arguments_to_parse));
      if (command_to_exec == NULL || arguments_to_parse == NULL) {
        fprintf(stderr, "MALLOC() failed!!\n");
        return 1;
      }

      // parses the arguments and gives back the number of them
      argument_parser(buffer, command_to_exec, arguments_to_parse, length_command_to_exec, &number_of_args);
      
      char **args = (char**) malloc(sizeof(char *) * number_of_args);
      if (args == NULL) {
        fprintf(stderr, "MALLOC() failed!!\n");
        return 1;
      }

      // creates the args matrix for the exec
      args_matrix_creation(args, command_to_exec, arguments_to_parse, number_of_args);

      // child creation + execution of command
      if (!fork()) {
        sleep(3);
        execvp(command_to_exec, args);
      }

      // we make the parent wait so everything goes smoothly
      wait((int*) 0);
    }
  }

  return 0;
}

// properly parses the arguments
void argument_parser(char *buffer, 
                     char *command_to_exec, 
                     char *arguments_to_parse, 
                     size_t length_command_to_exec, 
                     size_t *number_of_args) {
  size_t n_args = 2;
  size_t length_arguments_to_parse = strlen(buffer) - length_command_to_exec;
 
  // command to exec, argument parsing + matrix creation
  for (size_t i = 0; i < length_command_to_exec; i++)
    command_to_exec[i] = buffer[i];
  for (size_t i = 0; i < length_arguments_to_parse; i++)
    arguments_to_parse[i] = buffer[i + length_command_to_exec];
  for (size_t i = 0; i < length_arguments_to_parse; i++)
    if (arguments_to_parse[i] == ' ') n_args++;
  
  *number_of_args = n_args;
}

// moves arguments into args matrix
void args_matrix_creation(char **args, 
                          char *command_to_exec,
                          char *arguments_to_parse,
                          size_t number_of_args) {
  size_t dummy = 0, index_args = 0;

  for (size_t i = 0; i <= strlen(arguments_to_parse); i++) {
    if (arguments_to_parse[i] == ' ' 
        || arguments_to_parse[i] == '\n' 
        || arguments_to_parse[i] == '\0'
        || i == 0) {

      if (i == 0)
        args[index_args] = (char *) malloc( sizeof(char) * (strlen(command_to_exec)));
      else 
        args[index_args] = (char *) malloc( sizeof(char) * (dummy));

      if (args[index_args] == NULL) {
        fprintf(stderr, "MALLOC FAILED!!\n");
        exit(1);
      }

      if (i == 0) {
        args[index_args] = command_to_exec;
      }
      else {
        for (int j = dummy; j >= 0; j--) {
          args[index_args][dummy - j] = arguments_to_parse[i - j];
          if (j == 0) args[index_args][dummy - j] = '\0';
        }
      }
          
      index_args++;
      dummy = 0;
    }
    else {
      dummy++;
    }
  }

  // exec wants the last argument to be NULL
  args[number_of_args - 1] = NULL;
}