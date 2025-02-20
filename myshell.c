// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed:___Ryan Dang_________ Date:_____10/15_____

// 3460:426 Lab 1 - Basic C shell rev. 9/10/2020

/* Basic shell */

/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv). Since
 * it uses "." (dot) as a separator, it cannot handle file
 * names like "minishell.h"
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>

#define MAX_ARGS		64
#define MAX_ARG_LEN		16
#define MAX_LINE_LEN	80
#define WHITESPACE		" ,\t\n"

struct command_t {
   char *name;
   int argc;
   char *argv[MAX_ARGS];
};

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);
void runCommand(struct command_t *);
void printManual();

int main(int argc, char *argv[]) {
   char cmdLine[MAX_LINE_LEN];
   struct command_t command;

   while (1) {
      printPrompt();
      /* Read the command line and parse it */
      readCommand(cmdLine);
      parseCommand(cmdLine, &command);
      command.argv[command.argc] = NULL;

      // check for shortcuts and build new commands if necessary
      const char *name = command.name;
      // copy file
      if (strcmp(name, "C") == 0) {
         command.name = "cp";
      }
      // delete file
      else if (strcmp(name, "D") == 0) {
         command.name = "rm";
      }
      // echo comment
      else if (strcmp(name, "E") == 0) {
         command.name = "echo";
      }
      // help manual
      else if (strcmp(name, "H") == 0) {
         printManual();
         command.name = '\0';
      }
      // modified ls command
      else if (strcmp(name, "L") == 0) {
         // print working directory
         char cwd[1024];
         getcwd(cwd, 1024);
         printf("\n%s\n", cwd);
         // use ls -l command
         command.name = "ls";
         command.argv[1] = "-l";
      }
      // make file with nano editor
      else if (strcmp(name, "M") == 0) {
         command.name = "nano";
      }
      // print file with more
      else if (strcmp(name, "P") == 0) {
         command.name = "more";
      }
      // exit shell
      else if (strcmp(name, "Q") == 0) {
         break;
      }
      // clear screen
      else if (strcmp(name, "W") == 0) {
         command.name = "clear";
      }
      // execute program
      else if (strcmp(name, "X") == 0) {
         // replace X with program name
         command.name = command.argv[1];
         // remove program name from argv
         command.argv[1] = '\0';
      }

      /* Create a child process to execute the command */
      runCommand(&command);
   }

   /* Shell termination */
   printf("\n\n shell: Terminating successfully\n");
   return 0;
}

/* End basic shell */

/* Parse Command function */

/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd) {
   int argc;
   char **clPtr;
   /* Initialization */
   clPtr = &cLine;	/* cLine is the command line */
   argc = 0;
   cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
   /* Fill argv[] */
   while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
      cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
   }

   /* Set the command name and argc */
   cmd->argc = argc-1;
   cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
   strcpy(cmd->name, cmd->argv[0]);
   return 1;
}

/* End parseCommand function */

/* Print prompt and read command functions - Nutt pp. 79-80 */

void printPrompt() {
   /* Build the prompt string to have the machine name,
    * current directory, or other desired information
    */
   // print current username
   struct passwd *p = getpwuid(getuid());
   printf("%s:", p->pw_name);
   // print current directory
   char cwd[1024];
   getcwd(cwd, 1024);
   printf("%s $ ", cwd);
}

void readCommand(char *buffer) {
   /* This code uses any set of I/O functions, such as those in
    * the stdio library to read the entire command line into
    * the buffer. This implementation is greatly simplified,
    * but it does the job.
    */
   fgets(buffer, 80, stdin);
}

/* End printPrompt and readCommand */

// run command on child process
void runCommand(struct command_t *command) {
   int pid;
   /* Create a child process to execute the command */
   if ((pid = fork()) == 0) {
      /* Child executing command */
      execvp(command->name, command->argv);
   }
   /* Wait for the child to terminate */
   wait(NULL);
}

// print help manual for user
void printManual() {
   printf(
      "myshell - basic linux shell with some additional shortcuts\n\n"
      "BASIC FUNCTIONALITY\n"
      "This shell can run the normal linux commands. For a list of commands, type \"help\" into the shell.\n"
      "This shell has additional shortcuts on top of the basic functionality.\n"
      "SHORTCUTS\n"
      "C file1 file2 - Copy file1 to file2. Creates or overwrites file2. file1 is untouched.\n"
      "D file - Delete the file.\n"
      "E comment - Echo. Display comment on screen.\n"
      "H - Help. Display user manual. What you're viewing right now.\n"
      "L - List. Modified version of ls. Prints working directory and lists contents in long form.\n"
      "M file - Make. Open or create a text file with nano editor.\n"
      "P file - Print file contents on screen with the more command.\n"
      "Q - Quit and terminate shell.\n"
      "W - Wipe and clear the screen.\n"
      "X program - Executes the program. Return to shell after exiting program.\n"
   );
}
