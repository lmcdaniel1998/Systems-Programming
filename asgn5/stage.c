#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stage.h"

Stage makeStage(int num) {
   Stage st;
   st.stageNum = num;
   strcpy(st.input, "original stdin");
   strcpy(st.output, "original stdout");
   st.argc = 0;
   st.next = NULL;
   return st;
}

void printStage(Stage st) {
   int i = 0;
   printf("\n--------\nStage %d: \"%s\"\n--------\n", st.stageNum, st.command);
   printf("     input: %s\n    output: %s\n", st.input, st.output);
   printf("      argc: %d\n", st.argc);
   printf("      argv: ");
   for (i = 0; i < st.argc; i++) {
      printf("\"%s\"", st.argv[i]);
      if ((i + 1) != st.argc)
         printf(",");
   }
   printf("\n");
}

Stage addStage(Stage *head, int num) {
   
   Stage new;
   if (head == NULL)
      return makeStage(0);
   
   new = makeStage(num);
   head->next = &new;
   return new;
}

/* return -1 if no < else return i */
int checkIn(char *allarg[15], int argc) {
  int i;
  for (i = 0; i < argc; i ++) {
    if(strcmp(allarg[i], "<") == 0) {
      return i;
    }
  }
  return -1;
}

/* return -1 if no < else return i */
int checkOut(char *allarg[15], int argc) {
  int i;
  for (i = 0; i < argc; i ++) {
    if(strcmp(allarg[i], ">") == 0) {
      return i;
    }
  }
  return -1;
}

Stage createStages(char *arg[256], int numArgs, int numStages) {
   
   int i = 0;
   char *allArg[15];
   char *input;
   int argc;
   int cur = 0;
   Stage head;
   Stage temp;
   
   while(cur < numStages) {
      argc = 0;
      /* create new stage and add to list */
      if (!cur) {
         head = addStage(&head, cur);
         temp = head;
      }
      else {
         temp = addStage(&temp, cur);
      }

      memset(&temp.command[0], 0, sizeof(temp.command));

      while (i < numArgs) {
         /* break when reach end of stage */
         if (strcmp(arg[i], "|") == 0) {
            i ++;
            break;
         }
         else {
            if((i + 1) < numArgs) {
              /* if < is present redirect input */
              if(strcmp(arg[i], "<") == 0) {
                input = arg[i + 1];
                printf("%s\n", input);
              }
            }
            strcat(temp.command, arg[i]);
            allArg[argc] = arg[i];
            argc ++;
         }
         i ++;
      }
      
      temp.argc = argc;
      temp.argv = allArg;
      temp.next = NULL;
      if (i != numArgs) 
         snprintf(temp.output, 30,  "pipe to stage %d", cur + 1);
      
      if (cur)
         snprintf(temp.input, 30, "pipe from stage %d", cur - 1);

      printStage(temp);
      cur ++;
   }
   if (allArg == NULL) {
      return head;
   }
   return head;
}
