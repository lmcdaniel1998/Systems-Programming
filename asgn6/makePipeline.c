#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include "stage.h"

#define READ_END 0
#define WRITE_END 1

int myCD(char *pth) {
   if (chdir(pth) == -1) {
      perror(pth);
      return 1;
   }
   return 0;
}

int getNumStages(Stage *st) {
   Stage *temp;
   int num = 0;
   temp = st;
   while (temp != NULL) {
      temp = temp->next;
      num++;
   }
   return num;
}

void closePipes(int pipe_ends[11][2], int numStages) {
   int i;
   for (i = 0; i < (numStages -1); i ++) {
      close(pipe_ends[i][READ_END]);
      close(pipe_ends[i][WRITE_END]);
   }
}

int pipeschild(Stage *st) {
    
   pid_t child;
   int numStages = getNumStages(st);
   int curStage = 0;
   int pipe_ends[11][2];
   int i, status;

   if (strcmp(st->argv[0], "cd") == 0) {
      if (myCD(st->argv[1]) == 1) {
         return 1;
      }
      return 0;
   }

   if (numStages > 1) {

      /* Create Pipes */
      for (i = 0; i < (numStages - 1); i++) {
         if (pipe(pipe_ends[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
         }
      }

      while (curStage != numStages) {
         
         if (!(child = fork())) {
            /* child functionality */
            
            /* First Stage */
            if (curStage == 0) {
            
               if (strcmp(st->input, "stdin") != 0) {
                  if (dup2(open(st->input, O_RDONLY), STDIN_FILENO) == -1) {
                     perror("dup2");
                     exit(EXIT_FAILURE);
                  }
               }
               if (dup2(pipe_ends[curStage][WRITE_END], STDOUT_FILENO) == -1) {
                  perror("dup2");
                  exit(EXIT_FAILURE);
               }
            }

            /* Last Stage */
            else if ((curStage + 1) == numStages) {
               if (strcmp(st->output, "stdout") != 0) {
                  if (dup2(open(st->output, O_WRONLY | O_TRUNC | O_CREAT, 0666), 
                     STDOUT_FILENO) == -1) {
                     
                     perror("dup2");
                     exit(EXIT_FAILURE);
                  }
               }
               if (dup2(pipe_ends[curStage - 1][READ_END], STDIN_FILENO) == -1) {
                  perror("dup2");
                  exit(EXIT_FAILURE);
               }
            }

            /* Middle Stage */
            else {
               
               if (dup2(pipe_ends[curStage - 1][READ_END], STDIN_FILENO) == -1) {
                  perror("dup2");
                  exit(EXIT_FAILURE);
               }
               if (dup2(pipe_ends[curStage][WRITE_END], STDOUT_FILENO) == -1) {
                  perror("dup2");
                  exit(EXIT_FAILURE);
               }
            }

            closePipes(pipe_ends, numStages);
            execvp(st->argv[0], st->argv);
            perror(st->argv[0]);
            exit(EXIT_FAILURE);
         }
         curStage ++;
         st = st->next;
      }

      closePipes(pipe_ends, numStages);
      
      while(numStages > 0) {
         if(-1 == wait(&status)) {
            perror("wait");
         }
         numStages --;
      }
   }

   else {
      
      pipe_ends[0][READ_END] = STDIN_FILENO;
      pipe_ends[0][WRITE_END] = STDOUT_FILENO;
      
      if((child = fork()) == 0) {
         /* child stuff */
         
         if (strcmp(st->input, "stdin") != 0) {
            if ((pipe_ends[0][READ_END] = open(st->input, O_RDONLY)) == -1) {
               perror(st->input);
               return 1;
            }
            else {
               if (dup2(pipe_ends[0][READ_END], STDIN_FILENO) == -1) {
                  perror("dup2");
                  exit(EXIT_FAILURE);
               }
            }
         }

         if (strcmp(st->output, "stdout") != 0) {
            if ((pipe_ends[0][WRITE_END] = open(st->output, 
               O_WRONLY | O_TRUNC | O_CREAT, 0666)) == -1) {
               perror(st->output);
               return 1;
            }
            else {
               if (dup2(pipe_ends[0][WRITE_END], STDOUT_FILENO) == -1) {
                  perror("dup2");
                  exit(EXIT_FAILURE);
               }
            }
         }
         execvp(st->argv[0], st->argv);
         perror(st->argv[0]);
         return 1;
      }
      wait(NULL);
   }
   return 0;
}
