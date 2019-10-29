#ifndef STAGE_H
#define STAGE_H

typedef struct Stage {
   char command[512];
   int stageNum;
   char input[30];
   char output[30];
   int argc;
   char **argv;
   struct Stage *next;
} Stage;

Stage createStages(char *arg[256], int numArgs, int numStages);

#endif
