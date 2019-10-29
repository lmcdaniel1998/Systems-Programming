#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "stage.h"
#include "parseMain.h"

/* checks if all uses of < (input redirection) are valid */

int firstStageEnd(char *args[], int numStages, int numArgs) {
  int i = 0;

  /* if only one stage return 0 ( < is always valid ) */
  if (numStages <= 1) {
    return 0;
  }

  /* cycle through list until first | is found */
  while (strcmp(args[i], "|") != 0) {
    i ++;
  }
  /* return index where first stage ends */
  return i;
}

void freeStages(Stage *st) {
   Stage *temp;
   int i;
   while(st != NULL) {
      temp = st;
      for (i = 0; i < temp->argc; i++) {
         free(temp->argv[i]);
      }
      free(temp->argv);
      free(temp->input);
      free(temp->output);
      st = st->next;
      free(temp);
   }
}

int checkInputs(char *args[], int numArgs, int numStages) {

  int i;
  int carets = 0;
  int endFirst;
  int is_caret;

  if (strcmp(args[0], "<") == 0) {
    printf("%s: invalid null command", args[0]);
    return 1;
  }

  /* find end of first stage */
  endFirst = firstStageEnd(args, numStages, numArgs);

  /* loop through all args */
  for (i = 1; i  < numArgs; i++) {
     /* count carets */
     is_caret = 0;
     if (strcmp(args[i], "<") == 0) {
       is_caret = 1;
       carets ++;
       if (carets > 1) {
          printf("%s: bad input redirection\n", args[0]);
          return 1;
       }
       else if ((i + 1) == numArgs) {
          printf("%s: bad input redirection\n", args[0]);
          return 1;
       }
       else if (strcmp(args[i + 1], "|") == 0) {
          printf("%s: bad input redirection\n", args[0]);
          return 1;
       }
       else if ( (strcmp(args[i - 1], "<") == 0) ||
          (strcmp(args[i + 1], "<") == 0) ) {
          
          printf("%s: bad input redirection\n", args[0]);
          return 1;
       }
       else if ( (strcmp(args[i - 1], ">") == 0) ||
          (strcmp(args[i + 1], ">") == 0) ) {
          
          printf("%s: bad input redirection\n", args[0]);
          return 1;
       }
     }
     /* ambiguous input */
     if (numStages > 1) {
       if ((is_caret == 1) && (i > endFirst)) {
         printf("%s: ambiguous input\n", args[0]);
         return 1;
       }
     }
  }
  return 0;
}

int checkOutput(char *args[256], int numArgs, int numStages) {
   
   int numPipe = 0;
   int i;
   int numDir = 0;
   if (strcmp(args[0], ">") == 0) {
      printf("invalid null command\n");
      return 1;
   }
   
   for (i = 1; i < numArgs; i++) {
      if (strcmp(args[i], ">") == 0) {
         numDir ++;
         if ((numPipe !=  (numStages -1)) && (numStages != 1)) {
            printf("%s: ambiguous output\n", args[0]);
            return 1;
         }
         else if (numDir > 1) {
            printf("%s: bad output redirection\n", args[0]);
            return 1;
         }

         else if ( (i + 1) == numArgs) {
            printf("%s: bad output redirection\n", args[0]);
            return 1;
         }
         else if (strcmp(args[i + 1], "|") == 0) {
            printf("%s: bad output redirection\n", args[0]);
            return 1;
         }
         else if ( (strcmp(args[i - 1], "<") == 0) ||
            (strcmp(args[i + 1], "<") == 0) ) {
            
            printf("%s: bad output redirection\n", args[0]);
            return 1;
         }
         else if ( (strcmp(args[i - 1], ">") == 0) ||
            (strcmp(args[i + 1], ">") == 0) ) {
            
            printf("%s: bad output redirection\n", args[0]);
            return 1;
         }
      }
      else if (strcmp(args[i], "|") == 0)
         numPipe ++;
   } 
   return 0;
}

/* Checks if there is an argument before and after each pipe */
int checkPipes(char *args[256], int numArgs, int numStages) {
   
  int i = 0;
  if (strcmp(args[0], "|") == 0) {
     printf("invalid null command\n");
     return 1;
  }

  for (i = 1; i < numArgs; i ++) {
     if (strcmp(args[i], "|") == 0) {
        if ( ((i+1) == numArgs)) {
           printf("invalid null command\n");
           return 1;
        }
        else if (strcmp(args[i - 1], "|") == 0) {
           printf("invalid null command\n");
           return 1;
        }
        else if (strcmp(args[i + 1], "|") == 0) {
           printf("invalid null command\n");
           return 1;
        }
     }
  }
  return 0;
}

int checkCD(char *arg[256], int numArgs, int numStages) {
   int i = 0;
   while (i < numArgs) {
      if (strcmp("cd", arg[i]) == 0) {
         if ((i != 0) || (numStages != 1) || (numArgs != 2)) {
            return 1;
         }
      }
      i ++;
   }
   return 0;
}

int createArgs(char *command, char *args[256], int *numStages) {
   int count = 0;
   char *temp;
   temp = strtok(command, " ");
   while (temp != NULL) {
      
      if (strcmp(temp, "|") == 0) {
         (*numStages) ++;
      }
      args[count] = temp;
      count ++;
      temp = strtok(NULL, " ");
   }
   return count;
}
 
void handler2(int num) {
  fflush(stdout);
}

Stage *parseMain(FILE *fd) {
   char command[513];
   int count = 0, numArgs = 0;
   char c;
   char *listArg[256];
   int numStages = 1;
   char saved[513];

   struct sigaction sa;

   sa.sa_handler = handler2;
   sa.sa_flags = 0;

   sigemptyset(&sa.sa_mask);
   sigaction(SIGINT, &sa, NULL);

   while ((c = getc(fd)) != '\n') {
      if (c == EOF) {
         printf("\n");
         exit(EXIT_SUCCESS);
      }
      if (count < 512) {
         command[count] = c;
         count ++;
      }
   }
   if (count >= 512) {
      printf("command too long\n");
      return NULL;
   }

   command[count] = '\0';
   strcpy(saved, command);

   numArgs = createArgs(command, listArg, &numStages);
   
   if (numStages > 10) {
      perror("pipeline too deep");
      return NULL;
   }
   if (!numArgs) {
      return NULL;
   }
   
   if (numStages > 1) {
      if (checkPipes(listArg, numArgs, numStages)) {
         return NULL;
      }
   }
   if (checkOutput(listArg, numArgs, numStages) == 1) {
      return NULL;
   }
   if (checkInputs(listArg, numArgs, numStages) == 1) {
      return NULL;
   }
   if (checkCD(listArg, numArgs, numStages)) {
      perror("incorrect use of command 'cd'");
      return NULL;
   }
   return createStages(listArg, numArgs, numStages, saved);   
}
