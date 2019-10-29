#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stage.h"

Stage *makeStage(int num) {
   Stage *st = malloc(sizeof(Stage));
   st->stageNum = num;
   st->argv = malloc(11 * sizeof(char *));
   st->input = malloc(256 * sizeof(char));
   st->output = malloc(256 * sizeof(char));
   strcpy(st->input, "stdin");
   strcpy(st->output, "stdout");
   st->command[0] = '\0';
   st->argc = 0;
   st->next = NULL;
   return st;
}

Stage *createStages(char *arg[256], int numArgs,
   int numStages, char command[513]) {
   int i = 0;
   int argc;
   int cur = 0;
   Stage *head;
   Stage *temp;
   char *com;
   com = strtok(command, "|");

   while(cur < numStages) {
      argc = 0;
      if (!cur) {
         head = makeStage(cur);
         temp = head;
      }
      else {
         temp->next = makeStage(cur);
         temp = temp->next;
      }

      if (com) {
         strcpy(temp->command, com);
         com = strtok(NULL, "|");
      }
      while (i < numArgs) {

         if (strcmp(arg[i], "|") == 0) {
            i ++;
            strcpy(temp->output, arg[i]);
            break;
         }
         else {
            
            if (strcmp(arg[i], "<") == 0) {
               i ++;
               strcpy(temp->input, arg[i]);
            }
            else if (strcmp(arg[i], ">") == 0) {
               i ++;
               strcpy(temp->output, arg[i]);
            }
            else {
               temp->argv[argc] = malloc((strlen(arg[i])+1) * sizeof(char));
               strcpy(temp->argv[argc], arg[i]);
               argc ++;
               if (argc > 10) {
                  printf("%s: too many arguments\n", temp->argv[0]);
                  exit(EXIT_FAILURE);
               }
            }
         }
         i ++;
      }
      temp->argv[argc] = NULL;
      temp->argc = argc;
      temp->next = NULL;
      /*
      if (i != numArgs) 
         snprintf(temp->output, 30, "%d",  cur + 1);
      */
      if (cur)
         snprintf(temp->input, 30, "%d",  cur - 1);

      cur ++;
   }
   if (argc == 0) {
      return head;
   }
   return head;
}
